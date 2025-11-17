// Headers/MatchingEngine.h
#pragma once

#include <vector>

#include "OrderBook.h"

class MatchingEngine {
 public:
  MatchingEngine() = default;

  // ← only a declaration, no body here
  std::vector<Trade> match();
  OrderBook& getOrderBook() { return internal_order_book_; }

 private:
  OrderBook internal_order_book_;
};
