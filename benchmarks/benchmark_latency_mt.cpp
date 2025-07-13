#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "MatchingEngine.h"
#include "OrderBook.h"

Order generateRandomOrder() {
  static std::atomic<int64_t> globalOrderCounter{1};
  static thread_local std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<double> price_dist(10.0, 100.0);
  std::uniform_int_distribution<int> quantity_dist(1, 100);
  std::uniform_int_distribution<int> side_dist(0, 1);

  double price = price_dist(rng);
  int quantity = quantity_dist(rng);
  OrderSide side = static_cast<OrderSide>(side_dist(rng));

  std::ostringstream oss;
  oss << "ORD-" << std::setw(8) << std::setfill('0') << globalOrderCounter++;
  std::string id = oss.str();

  return Order(id, side, price, quantity);
}

void run_latency_benchmark_mt(int num_orders) {
  std::vector<double> latencies_ms;
  std::mutex latency_mutex;

  OrderBook book;
  MatchingEngine engine(book);

  std::vector<std::thread> threads;
  unsigned int thread_count = std::thread::hardware_concurrency();
  std::cout << "Launching " << thread_count
            << " threads (std::thread::hardware_concurrency)\n";

  auto worker = [&](int num_ops) {
    for (int i = 0; i < num_ops; ++i) {
      Order order = generateRandomOrder();
      auto start = std::chrono::high_resolution_clock::now();
      book.addOrder(order);
      engine.match();
      auto end = std::chrono::high_resolution_clock::now();

      double latency =
          std::chrono::duration<double, std::milli>(end - start).count();

      std::lock_guard<std::mutex> lock(latency_mutex);
      latencies_ms.push_back(latency);
    }
  };

  int ops_per_thread = num_orders / thread_count;
  int remainder = num_orders % thread_count;

  for (unsigned int t = 0; t < thread_count; ++t) {
    int ops = ops_per_thread + (t < remainder ? 1 : 0);
    threads.emplace_back(worker, ops);
  }

  for (auto& th : threads) {
    th.join();
  }

  std::sort(latencies_ms.begin(), latencies_ms.end());
  double mean = std::accumulate(latencies_ms.begin(), latencies_ms.end(), 0.0) /
                latencies_ms.size();
  double p50 = latencies_ms[latencies_ms.size() * 0.5];
  double p95 = latencies_ms[latencies_ms.size() * 0.95];
  double p99 = latencies_ms[latencies_ms.size() * 0.99];
  double max_latency = latencies_ms.back();

  std::cout << "Latency Stats (ms):\n";
  std::cout << "  Mean: " << mean << "\n";
  std::cout << "  P50:  " << p50 << "\n";
  std::cout << "  P95:  " << p95 << "\n";
  std::cout << "  P99:  " << p99 << "\n";
  std::cout << "  Max:  " << max_latency << "\n";

  std::ofstream csv("latency_mt_data.csv");
  csv << "latency_ms\n";
  for (double lat : latencies_ms) {
    csv << std::fixed << std::setprecision(6) << lat << "\n";
  }
  csv.close();

  std::cout << "[✔] Latency data written to latency_mt_data.csv\n";
}

int main() {
  int num_orders = 20000;
  run_latency_benchmark_mt(num_orders);
  return 0;
}
