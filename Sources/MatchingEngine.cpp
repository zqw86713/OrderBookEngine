#include "../Headers/MatchingEngine.h"

std::vector<Trade> MatchingEngine::match() {
    std::vector<Trade> trades;

    while (true) {
        auto top_bid_opt = order_book_.getTopBid();
        auto top_ask_opt = order_book_.getTopAsk();

        if (!top_bid_opt.has_value() || !top_ask_opt.has_value()) break;

        const Order& top_bid = *top_bid_opt;
        const Order& top_ask = *top_ask_opt;

        if (top_bid.price < top_ask.price) break;

        int trade_qty = std::min(top_bid.quantity, top_ask.quantity);
        double trade_price = top_ask.price;

        trades.emplace_back(Trade{
            top_bid.id,
            top_ask.id,
            trade_price,
            trade_qty,
            std::chrono::steady_clock::now()
            });

        int remaining_bid_qty = top_bid.quantity - trade_qty;
        int remaining_ask_qty = top_ask.quantity - trade_qty;

        // Remove both orders
        order_book_.cancelOrder(top_bid.id);
        order_book_.cancelOrder(top_ask.id);

        // Re-add remaining bid with fresh timestamp
        if (remaining_bid_qty > 0) {
            Order remaining_bid = top_bid;
            remaining_bid.quantity = remaining_bid_qty;
            remaining_bid.timestamp = std::chrono::steady_clock::now();  // 💡 critical
            order_book_.addOrder(remaining_bid);
        }

        // Re-add remaining ask with fresh timestamp
        if (remaining_ask_qty > 0) {
            Order remaining_ask = top_ask;
            remaining_ask.quantity = remaining_ask_qty;
            remaining_ask.timestamp = std::chrono::steady_clock::now();  // 💡 critical
            order_book_.addOrder(remaining_ask);
        }
    }

    return trades;
}
