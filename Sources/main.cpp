#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "../Headers/MatchingEngine.h"
#include "../Headers/OrderBook.h"

int main() {
  OrderBook order_book;
  MatchingEngine engine(order_book);

  // ???? 6 ??????????????
  order_book.addOrder(Order("buy1", OrderSide::BUY, 105.0, 10));
  order_book.addOrder(Order("sell1", OrderSide::SELL, 95.0, 10));
  order_book.addOrder(Order("buy2", OrderSide::BUY, 105.0, 10));
  order_book.addOrder(Order("sell2", OrderSide::SELL, 95.0, 10));
  order_book.addOrder(Order("buy3", OrderSide::BUY, 105.0, 10));
  order_book.addOrder(Order("sell3", OrderSide::SELL, 95.0, 10));

  std::cout << "[INFO] Starting match...\n";

  auto trades = engine.match();

  std::cout << "[RESULT] Trades executed: " << trades.size() << "\n";
  for (const auto& t : trades) {
    std::cout << "  - " << t.buyer_id << " buys from " << t.seller_id << " at "
              << t.price << " qty " << t.quantity << "\n";
  }

  return 0;
}
