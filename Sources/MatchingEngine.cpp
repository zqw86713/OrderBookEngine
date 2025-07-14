// Sources/MatchingEngine.cpp
#include "MatchingEngine.h"

#include <algorithm>  // 若不需要算法，可删掉
#include <chrono>     // 为 std::chrono::steady_clock::now()

#include "OrderBook.h"

// 如果你不想把 match() 全写在头里，也可以像下面这样单独实现：
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

    // 这里顺序要和 Trade 结构体一致：quantity 在 price 前面
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
