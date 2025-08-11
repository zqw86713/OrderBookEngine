// Sources/MatchingEngine.cpp
#include "MatchingEngine.h"

#include <algorithm>
#include <chrono>  //  std::chrono::steady_clock::now()

#include "OrderBook.h"

std::vector<Trade> MatchingEngine::match() {
  std::vector<Trade> trades;

  while (true) {
    auto bid_opt = order_book_.getTopBid();
    auto ask_opt = order_book_.getTopAsk();
    if (!bid_opt.has_value() || !ask_opt.has_value()) break;

    const Order& bid = *bid_opt;
    const Order& ask = *ask_opt;
    if (bid.price < ask.price) break;

    int qty = std::min(bid.quantity, ask.quantity);
    double price = ask.price;

    trades.push_back(
        Trade{bid.id, ask.id, qty, price, std::chrono::steady_clock::now()});

    order_book_.cancelOrder(bid.id);
    order_book_.cancelOrder(ask.id);

    if (bid.quantity > qty) {
      Order r = bid;
      r.quantity = bid.quantity - qty;
      r.timestamp = std::chrono::steady_clock::now();
      order_book_.addOrder(r);
    }
    if (ask.quantity > qty) {
      Order r = ask;
      r.quantity = ask.quantity - qty;
      r.timestamp = std::chrono::steady_clock::now();
      order_book_.addOrder(r);
    }
  }

  return trades;
}
