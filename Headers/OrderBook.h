#pragma once
#include "Order.h"
#include <unordered_map>
#include <set>
#include <string>
#include <optional>


class OrderBook {
public:
	bool addOrder(const Order& order);
    bool cancelOrder(const std::string& orderID);
	bool modifyOrder(const std::string& order_id, double new_price, int new_quantity);
	void matchOrders();

	std::optional<Order> getTopBid() const;
	std::optional<Order> getTopAsk() const;
	

private:
	std::unordered_map<std::string, std::set<Order>::iterator> order_lookup_;
	std::set<Order, BidComparator>	buy_orders_;
	std::set<Order, AskComparator>	sell_orders_;
};

