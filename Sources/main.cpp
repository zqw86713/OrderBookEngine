#include "../Headers/OrderBook.h"
#include "../Headers/MatchingEngine.h"
#include <iostream>

int main() {
    OrderBook order_book;


    order_book.addOrder(Order("B1", OrderSide::BUY, 101.0, 10));
    order_book.addOrder(Order("B2", OrderSide::BUY, 100.5, 5));
    order_book.addOrder(Order("S1", OrderSide::SELL, 99.5, 8));
    order_book.addOrder(Order("S2", OrderSide::SELL, 100.0, 7));


    MatchingEngine engine(order_book);
    auto trades = engine.match();
       


    for (const auto& trade : trades) {
        std::cout << "Trade: "
            << trade.buyer_id << " buys from " << trade.seller_id
            << " at $" << trade.price << " for " << trade.quantity
            << " units\n";
    }

    return 0;
}
