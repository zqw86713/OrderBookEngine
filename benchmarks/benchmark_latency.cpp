#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <random>
#include <string>
#include <sstream>
#include <numeric>
#include "OrderBook.h"
#include "MatchingEngine.h"

// ----- 生成随机订单 -----
Order generateRandomOrder() {
    static int64_t globalOrderCounter = 1;
    static std::mt19937 rng(std::random_device{}());

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

// ----- 延迟测试逻辑 -----
void run_latency_benchmark(int num_orders) {
    std::vector<double> latencies_ms;
    OrderBook book;
    MatchingEngine engine(book);

    for (int i = 0; i < num_orders; ++i) {
        Order order = generateRandomOrder();
        book.addOrder(order);  // 你需要在 OrderBook 中实现这个函数

        auto start = std::chrono::high_resolution_clock::now();
        engine.match();  // 无参调用
        auto end = std::chrono::high_resolution_clock::now();

        double latency = std::chrono::duration<double, std::milli>(end - start).count();
        latencies_ms.push_back(latency);
    }

    std::sort(latencies_ms.begin(), latencies_ms.end());
    double mean = std::accumulate(latencies_ms.begin(), latencies_ms.end(), 0.0) / latencies_ms.size();
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

    std::ofstream csv("latency_data.csv");
    csv << "latency_ms\n";
    for (double lat : latencies_ms) {
        csv << std::fixed << std::setprecision(6) << lat << "\n";
    }
    csv.close();

    std::cout << "[✔] Latency data written to latency_data.csv\n";
}

int main() {
    int num_orders = 20000;
    run_latency_benchmark(num_orders);
    return 0;
}
