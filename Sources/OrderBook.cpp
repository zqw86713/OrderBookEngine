
// Sources/OrderBook.cpp
#include "OrderBook.h"

#include <algorithm>

bool OrderBook::addOrder(const Order& order) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);

  // Only reject duplicates
  if (buy_lookup_.count(order.id) || sell_lookup_.count(order.id)) return false;

  // Insert market or limit orders into book
  if (order.side == OrderSide::BUY) {
    auto it = buy_orders_.insert(order).first;
    buy_lookup_[order.id] = it;
  } else {
    auto it = sell_orders_.insert(order).first;
    sell_lookup_[order.id] = it;
  }
  return true;
}

bool OrderBook::cancelOrder(const std::string& order_id) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  auto ib = buy_lookup_.find(order_id);
  if (ib != buy_lookup_.end()) {
    buy_orders_.erase(ib->second);
    buy_lookup_.erase(ib);
    return true;
  }
  auto is = sell_lookup_.find(order_id);
  if (is != sell_lookup_.end()) {
    sell_orders_.erase(is->second);
    sell_lookup_.erase(is);
    return true;
  }
  return false;
}

bool OrderBook::modifyOrder(const std::string& order_id, double new_price,
                            int new_quantity) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  auto ib = buy_lookup_.find(order_id);
  if (ib != buy_lookup_.end()) {
    Order old = *ib->second;
    if (old.type == OrderType::MARKET) return false;
    cancelOrder(order_id);
    return addOrder(Order(order_id, old.side, new_price, new_quantity, old.type,
                          old.timestamp));
  }
  auto is = sell_lookup_.find(order_id);
  if (is != sell_lookup_.end()) {
    Order old = *is->second;
    if (old.type == OrderType::MARKET) return false;
    cancelOrder(order_id);
    return addOrder(Order(order_id, old.side, new_price, new_quantity, old.type,
                          old.timestamp));
  }
  return false;
}

std::optional<Order> OrderBook::getTopBid() const {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (buy_orders_.empty()) return std::nullopt;
  return *buy_orders_.begin();
}

std::optional<Order> OrderBook::getTopAsk() const {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (sell_orders_.empty()) return std::nullopt;
  return *sell_orders_.begin();
}

void OrderBook::matchOrders() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  while (!buy_orders_.empty() && !sell_orders_.empty()) {
    Order bid = *buy_orders_.begin();
    Order ask = *sell_orders_.begin();

    double exec_price = 0.0;
    bool can_match = false;
    if (bid.type == OrderType::MARKET) {
      exec_price = ask.price;
      can_match = true;
    } else if (ask.type == OrderType::MARKET) {
      exec_price = bid.price;
      can_match = true;
    } else if (bid.price >= ask.price) {
      exec_price = ask.price;
      can_match = true;
    }
    if (!can_match) break;

    int qty = std::min(bid.quantity, ask.quantity);
    Trade t{bid.id, ask.id, qty, exec_price, std::chrono::steady_clock::now()};
    reportTrade(t);

    cancelOrder(bid.id);
    cancelOrder(ask.id);

    if (bid.type == OrderType::LIMIT && bid.quantity > qty) {
      Order r{bid.id,           bid.side,
              bid.price,        bid.quantity - qty,
              OrderType::LIMIT, std::chrono::steady_clock::now()};
      addOrder(r);
    }
    if (ask.type == OrderType::LIMIT && ask.quantity > qty) {
      Order r{ask.id,           ask.side,
              ask.price,        ask.quantity - qty,
              OrderType::LIMIT, std::chrono::steady_clock::now()};
      addOrder(r);
    }
  }
}

void OrderBook::setTradeCallback(TradeCallback cb) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  trade_callback_ = std::move(cb);
}

const std::vector<Trade>& OrderBook::getExecutedTrades() const {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  return executed_trades_;
}

void OrderBook::reportTrade(const Trade& t) {
  executed_trades_.push_back(t);
  if (trade_callback_) trade_callback_(t);
}
