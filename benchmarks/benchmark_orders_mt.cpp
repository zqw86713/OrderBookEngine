#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "../Headers/MatchingEngine.h"
#include "../Headers/OrderBook.h"

// Generate synthetic orders with fixed price pattern for matchability
std::vector<Order> generateOrders(int count) {
  std::vector<Order> orders;
  orders.reserve(count);
  for (int i = 0; i < count; ++i) {
    OrderSide side = (i % 2 == 0) ? OrderSide::BUY : OrderSide::SELL;
    double price = (side == OrderSide::BUY) ? 105.0 : 95.0;
    orders.emplace_back("order_" + std::to_string(i), side, price,
                        10 + (i % 5));
  }
  return orders;
}

// Thread-safe insert wrapper
void insertOrders(OrderBook& ob, const std::vector<Order>& orders, int start,
                  int end) {
  for (int i = start; i < end; ++i) {
    ob.addOrder(orders[i]);
  }
}

int main() {
  std::cout << "[DEBUG] Program started.\n";

  const int num_orders = 20000;
  const int num_threads = 4;
  const int chunk_size = num_orders / num_threads;

  OrderBook order_book;
  MatchingEngine engine(order_book);
  auto orders = generateOrders(num_orders);

  std::cout << "[DEBUG] Orders generated: " << orders.size() << "\n";

  std::cout << "[INFO] Starting multithreaded addOrder...\n";

  auto t0 = std::chrono::high_resolution_clock::now();

  // Launch 4 threads to insert orders
  std::vector<std::thread> threads;
  for (int t = 0; t < num_threads; ++t) {
    int start = t * chunk_size;
    int end = (t == num_threads - 1) ? num_orders : (t + 1) * chunk_size;
    threads.emplace_back(insertOrders, std::ref(order_book), std::cref(orders),
                         start, end);
  }

  for (auto& th : threads) {
    th.join();
  }

  std::cout << "[INFO] Orders inserted. Starting match...\n";

  auto t1 = std::chrono::high_resolution_clock::now();
  auto trades = engine.match();
  auto t2 = std::chrono::high_resolution_clock::now();

  auto insert_us =
      std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
  auto match_us =
      std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
  auto total_us =
      std::chrono::duration_cast<std::chrono::microseconds>(t2 - t0).count();

  std::cout << "[RESULT] Insert time: " << insert_us / 1000 << " ms\n";
  std::cout << "[RESULT] Match time: " << match_us / 1000 << " ms\n";
  std::cout << "[RESULT] Total time: " << total_us / 1000 << " ms\n";
  std::cout << "[RESULT] Total trades: " << trades.size() << "\n";

  return 0;
}
