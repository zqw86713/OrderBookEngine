// ✅ 撮合延迟统计版本 benchmark_orders_v2.cpp
#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "../Headers/MatchingEngine.h"
#include "../Headers/OrderBook.h"

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

int main() {
  const int num_orders = 50000;
  OrderBook order_book;
  MatchingEngine engine(order_book);
  auto orders = generateOrders(num_orders);

  for (const auto& order : orders) {
    order_book.addOrder(order);
  }

  std::vector<long long> latencies;
  std::vector<Trade> all_trades;

  while (true) {
    auto top_bid = order_book.getTopBid();
    auto top_ask = order_book.getTopAsk();
    if (!top_bid.has_value() || !top_ask.has_value()) break;
    if (top_bid->price < top_ask->price) break;

    auto start = std::chrono::steady_clock::now();
    auto trades = engine.match();  // match() may produce multiple trades
    auto end = std::chrono::steady_clock::now();

    long long duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count();
    latencies.push_back(duration);
    all_trades.insert(all_trades.end(), trades.begin(), trades.end());
  }

  // Latency stats
  std::sort(latencies.begin(), latencies.end());
  auto p50 = latencies[latencies.size() * 0.5];
  auto p95 = latencies[latencies.size() * 0.95];
  auto p99 = latencies[latencies.size() * 0.99];

  std::cout << "✅ Matched " << num_orders << " orders in total\n";
  std::cout << "📈 Trades executed: " << all_trades.size() << "\n";
  std::cout << "📊 P50 latency: " << p50 << " μs\n";
  std::cout << "📊 P95 latency: " << p95 << " μs\n";
  std::cout << "📊 P99 latency: " << p99 << " μs\n";
}
