#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <thread>
#include <cstdlib>  // for std::atoi
#include "../Headers/OrderBook.h"
#include "../Headers/MatchingEngine.h"

// Generate synthetic orders
std::vector<Order> generateOrders(int count) {
    std::vector<Order> orders;
    orders.reserve(count);
    for (int i = 0; i < count; ++i) {
        OrderSide side = (i % 2 == 0) ? OrderSide::BUY : OrderSide::SELL;
        double price = (side == OrderSide::BUY) ? 105.0 : 95.0;
        orders.emplace_back("order_" + std::to_string(i), side, price, 10 + (i % 5));
    }
    return orders;
}

// Insert a slice of orders
void insertOrders(OrderBook& ob, const std::vector<Order>& orders, int start, int end) {
    for (int i = start; i < end; ++i) {
        ob.addOrder(orders[i]);
    }
}

int main(int argc, char* argv[]) {
    const int num_orders = 20000;
    int num_threads = 4;

    if (argc > 1) {
        num_threads = std::atoi(argv[1]);
        if (num_threads < 1) num_threads = 1;
    }

    int chunk_size = num_orders / num_threads;

    std::cout << "[INFO] Running with " << num_threads << " threads\n";
    std::cout << "[INFO] Generating " << num_orders << " orders...\n";

    auto orders = generateOrders(num_orders);
    OrderBook order_book;
    MatchingEngine engine(order_book);

    auto t0 = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    for (int t = 0; t < num_threads; ++t) {
        int start = t * chunk_size;
        int end = (t == num_threads - 1) ? num_orders : (t + 1) * chunk_size;
        threads.emplace_back(insertOrders, std::ref(order_book), std::cref(orders), start, end);
    }

    for (auto& th : threads) th.join();

    auto t1 = std::chrono::high_resolution_clock::now();
    std::cout << "[INFO] Orders inserted. Starting match...\n";

    auto trades = engine.match();
    auto t2 = std::chrono::high_resolution_clock::now();

    auto insert_us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
    auto match_us = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto total_us = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t0).count();

    std::cout << "[RESULT] Insert time: " << insert_us / 1000 << " ms\n";
    std::cout << "[RESULT] Match time: " << match_us / 1000 << " ms\n";
    std::cout << "[RESULT] Total time: " << total_us / 1000 << " ms\n";
    std::cout << "[RESULT] Total trades: " << trades.size() << "\n";
    std::cout << "[RESULT] Throughput: " << (num_orders * 1'000'000LL / total_us) << " orders/sec\n";

    return 0;
}
