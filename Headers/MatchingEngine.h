// Headers/MatchingEngine.h
#pragma once

#include <vector>

#include "OrderBook.h"  // 提供 Trade、OrderBook 定义

class MatchingEngine {
 public:
  MatchingEngine() = default;

  // ← only a declaration, no body here
  std::vector<Trade> match();
  OrderBook& getOrderBook() { return order_book_; }

 private:
  OrderBook order_book_;
};
