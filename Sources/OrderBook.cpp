#include "../Headers/OrderBook.h"
#include<algorithm>
#include <iostream>



bool OrderBook::addOrder(const Order& order) {
    std::lock_guard<std::mutex> lock(mutex_);

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
    std::lock_guard<std::mutex> lock(mutex_);

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
    std::lock_guard<std::mutex> lock(mutex_);

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
    std::lock_guard<std::mutex> lock(mutex_);

    if (buy_orders_.empty()) {
        return std::nullopt;
	}
	return *buy_orders_.begin();
}


std::optional<Order> OrderBook::getTopAsk() const {
    std::lock_guard<std::mutex> lock(mutex_);

    if (sell_orders_.empty()) {
        return std::nullopt;
    }
    return *sell_orders_.begin();
}


void OrderBook::matchOrders() {
    std::lock_guard<std::mutex> lock(mutex_);

    while (!buy_orders_.empty() && !sell_orders_.empty()) {
        auto best_bid_it = buy_orders_.begin();
        auto best_ask_it = sell_orders_.begin();

        const Order& best_bid = *best_bid_it;
        const Order& best_ask = *best_ask_it;

        // 成交条件
        if (best_bid.price >= best_ask.price) {
            int trade_qty = std::min(best_bid.quantity, best_ask.quantity);

            // ✅ 缓存剩余部分（在 cancelOrder 之前）
            int remaining_bid_qty = best_bid.quantity - trade_qty;
            int remaining_ask_qty = best_ask.quantity - trade_qty;

            std::string bid_id = best_bid.id;
            std::string ask_id = best_ask.id;

            // 输出成交信息
            //std::cout << "Trade executed: " << trade_qty << " units at price " << best_ask.price << "\n";

            // ⚠️ 删除旧订单（会使 iterator 失效）
            cancelOrder(bid_id);
            cancelOrder(ask_id);

            // ⚠️ 重建剩余挂单
            if (remaining_bid_qty > 0) {
                addOrder(Order(bid_id, OrderSide::BUY, best_bid.price, remaining_bid_qty, best_bid.timestamp));
            }

            if (remaining_ask_qty > 0) {
                addOrder(Order(ask_id, OrderSide::SELL, best_ask.price, remaining_ask_qty, best_ask.timestamp));
            }
        }
        else {
            break; // ❌ 不再满足撮合条件
        }
    }
}
