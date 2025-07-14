#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "../Headers/MatchingEngine.h"
#include "../Headers/OrderBook.h"

// Generate test orders: 50% BUY @ 105, 50% SELL @ 95
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
  const int num_orders = 20000;

  OrderBook order_book;
  MatchingEngine engine(order_book);
  auto orders = generateOrders(num_orders);

  for (const auto& order : orders) {
    order_book.addOrder(order);
  }

  std::vector<long long> batch_latencies;
  std::vector<Trade> all_trades;

  // Run match in batches (1 call = 1 batch)
  while (true) {
    auto top_bid = order_book.getTopBid();
    auto top_ask = order_book.getTopAsk();
    if (!top_bid.has_value() || !top_ask.has_value()) break;
    if (top_bid->price < top_ask->price) break;

    auto start = std::chrono::steady_clock::now();
    auto trades = engine.match();  // One batch
    auto end = std::chrono::steady_clock::now();

    long long duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count();
    batch_latencies.push_back(duration);
    all_trades.insert(all_trades.end(), trades.begin(), trades.end());
  }

  // Sort latency and compute stats
  std::sort(batch_latencies.begin(), batch_latencies.end());
  long long total_latency = 0;
  for (auto t : batch_latencies) total_latency += t;

  double avg = static_cast<double>(total_latency) / batch_latencies.size();
  long long p50 = batch_latencies[batch_latencies.size() * 0.5];
  long long p95 = batch_latencies[batch_latencies.size() * 0.95];
  long long p99 = batch_latencies[batch_latencies.size() * 0.99];

  std::cout << "✅ Total orders: " << num_orders << "\n";
  std::cout << "📈 Trades executed: " << all_trades.size() << "\n";
  std::cout << "📊 Batch count: " << batch_latencies.size() << "\n";
  std::cout << "⚡ Avg batch latency: " << avg << " μs\n";
  std::cout << "📊 P50: " << p50 << " μs\n";
  std::cout << "📊 P95: " << p95 << " μs\n";
  std::cout << "📊 P99: " << p99 << " μs\n";

  // Optional: export to CSV format
  std::cout << "\n[CSV] Batch#, Latency(μs)\n";
  for (size_t i = 0; i < batch_latencies.size(); ++i) {
    std::cout << i + 1 << "," << batch_latencies[i] << "\n";
  }

  return 0;
}
