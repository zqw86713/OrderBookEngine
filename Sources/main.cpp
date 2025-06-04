#include <iostream>
#include "../Headers/Order.h"
#include "../Headers/OrderBookEngine.h"


int main() {
  OrderBookEngine engine;

  std::cout << "--- Submitting Buy Order (ID 1) ---\n";
  Order buyOrder(1, OrderSide::BUY, 100.0, 10);
  engine.submitOrder(buyOrder);

  std::cout << "--- Submitting Sell Order (ID 2) ---\n";
  Order sellOrder(2, OrderSide::SELL, 99.0, 5);
  engine.submitOrder(sellOrder);  // Should match immediately

  std::cout << "--- Submitting Another Buy Order (ID 3) ---\n";
  Order buyOrder2(3, OrderSide::BUY, 101.0, 20);
  engine.submitOrder(buyOrder2);

  std::cout << "--- Canceling Buy Order (ID 3) ---\n";
  if (!engine.cancelOrder(3)) {
    std::cout << "Failed to cancel order ID 3\n";
  }

  std::cout << "--- Canceling Nonexistent Order (ID 999) ---\n";
  if (!engine.cancelOrder(999)) {
    std::cout << "Correctly failed to cancel non-existent order\n";
  }

  return 0;
}
