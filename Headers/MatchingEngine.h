// MatchingEngine.h
#pragma once

#include "OrderBook.h"
#include <vector>
#include <string>

struct Trade {
    std::string buyer_id;
    std::string seller_id;
    double price;
    int quantity;
    std::chrono::steady_clock::time_point timestamp;
};

class MatchingEngine {
public:
    MatchingEngine(OrderBook& order_book) : order_book_(order_book) {}

    // Perform matching and return list of trades
    std::vector<Trade> match();

private:
    OrderBook& order_book_;
};