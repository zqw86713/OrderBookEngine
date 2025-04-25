#pragma once
#include "order.h"
#include <queue>
#include <vector> 


class OrderBook {
public:
	void addOrder(const Order& order);
	void matchOrders();
    void cancelOrder(int orderID);



private:
	std::priority_queue<Order> buy_orders_;
	std::priority_queue<Order> sell_orders_;


};

