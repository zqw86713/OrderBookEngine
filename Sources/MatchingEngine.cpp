
// MatchingEngine.cpp
#include "../Headers/MatchingEngine.h"

std::vector<Trade> MatchingEngine::match() {
    std::vector<Trade> trades;

    while (true) {
        auto top_bid = order_book_.getTopBid();
        auto top_ask = order_book_.getTopAsk();

        if (!top_bid.has_value() || !top_ask.has_value()) break;
        if (top_bid->price < top_ask->price) break;

        int trade_qty = std::min(top_bid->quantity, top_ask->quantity);
        double trade_price = top_ask->price; // price-time priority (use ask price)

        trades.push_back(Trade{
            top_bid->id,
            top_ask->id,
            trade_price,
            trade_qty,
            std::chrono::steady_clock::now()
            });

        order_book_.cancelOrder(top_bid->id);
        order_book_.cancelOrder(top_ask->id);

        if (top_bid->quantity > trade_qty) {
            Order remaining_bid = *top_bid;
            remaining_bid.quantity -= trade_qty;
            order_book_.addOrder(remaining_bid);
        }
        if (top_ask->quantity > trade_qty) {
            Order remaining_ask = *top_ask;
            remaining_ask.quantity -= trade_qty;
            order_book_.addOrder(remaining_ask);
        }
    }

    return trades;
}
