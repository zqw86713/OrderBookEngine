#include "../Headers/OrderBook.h"
#include<algorithm>
#include <iostream>



bool OrderBook::addOrder(const Order& order) {
    if (order_lookup_.count(order.id)) {
        return false;
    }

    if (order.side == OrderSide::BUY) {
        auto it = buy_orders_.insert(order).first;
		order_lookup_[order.id] = it;
    } else {
		auto it = sell_orders_.insert(order).first;
        order_lookup_[order.id] = it;
    }
    	return true;
}


bool OrderBook::cancelOrder(const std::string& order_id) {
    auto it = order_lookup_.find(order_id);
    if (it == order_lookup_.end()) return false;

    const Order& order = *(it->second);
    if (order.side == OrderSide::BUY) {
        buy_orders_.erase(it->second);
    }
    else {
        sell_orders_.erase(it->second);
    }

    order_lookup_.erase(it);
    return true;
}


bool OrderBook::modifyOrder(const std::string& order_id, double new_price, int new_quantity) {
    auto it = order_lookup_.find(order_id);
    if (it == order_lookup_.end()) {
        return false;
    }

	Order old_order = *(it->second);
    cancelOrder(order_id);


    Order modified_order(order_id, old_order.side, new_price, new_quantity, old_order.timestamp);
	return addOrder(modified_order);

}


std::optional<Order> OrderBook::getTopBid() const {
    if (buy_orders_.empty()) {
        return std::nullopt;
	}
	return *buy_orders_.begin();
}


std::optional<Order> OrderBook::getTopAsk() const {
    if (sell_orders_.empty()) {
        return std::nullopt;
    }
    return *sell_orders_.begin();
}


void OrderBook::matchOrders() {
        while (!buy_orders_.empty() && !sell_orders_.empty()) {
			auto best_bid = *buy_orders_.begin();
			auto best_ask = *sell_orders_.begin();

            if (best_bid.price >= best_ask.price) {
                int trade_qty = std::min(best_bid.quantity, best_ask.quantity);
				// here you can log the trade or notify subscribers
                std::cout << "Trade executed: " << trade_qty << " units at price "
					<< best_ask.price << "\n";

				//Modify quantities or remove orders
				cancelOrder(best_bid.id);
                cancelOrder(best_ask.id);

                if (best_bid.quantity > trade_qty) {
					addOrder(Order(best_bid.id, best_bid.side, best_bid.price, best_bid.quantity - trade_qty, best_bid.timestamp));
                }

                if (best_ask.quantity > trade_qty) {
					addOrder(Order(best_ask.id, best_ask.side, best_ask.price, best_ask.quantity - trade_qty, best_ask.timestamp));

                }

            } else {
				break; // No more matches possible
            }
		}

    }