#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include "../Headers/OrderBook.h"
#include "../Headers/MatchingEngine.h"

// Generate synthetic test orders
std::vector<Order> generateOrders(int count) {
    std::vector<Order> orders;
    orders.reserve(count);

    for (int i = 0; i < count; ++i) {
        // 强制制造 match：BUY 105, SELL 95
        OrderSide side = (i % 2 == 0) ? OrderSide::BUY : OrderSide::SELL;
        double price = (side == OrderSide::BUY) ? 105.0 : 95.0;

        orders.emplace_back(
            "order_" + std::to_string(i),
            side,
            price,
            10 + (i % 5)
        );
    }
    return orders;
}


int main() {
    const int num_orders = 50000;

    OrderBook order_book;
    MatchingEngine engine(order_book);
    auto orders = generateOrders(num_orders);

    std::cout << "[INFO] Adding orders...\n";
    for (const auto& order : orders) {
        order_book.addOrder(order);
    }

    std::cout << "[INFO] Starting match...\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto trades = engine.match();
    std::cout << "[DEBUG] match() returned trades.size(): " << trades.size() << "\n";

    auto end = std::chrono::high_resolution_clock::now();
    auto total_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "✅ Matched " << num_orders << " orders in " << total_us << " μs\n";
    std::cout << "⚡ Throughput: " << (num_orders * 1'000'000LL / total_us) << " orders/sec\n";
    std::cout << "📈 Trades executed: " << trades.size() << "\n";


    return 0;
}
