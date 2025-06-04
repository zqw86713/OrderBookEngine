#pragma once
#include <string>
#include <chrono>

// This is the code that written yesterday.
enum class OrderSide {
	BUY,
	SELL
};



struct Order {
	std::string id; // Unique identifier for the order
	OrderSide side; // BUY or SELL side of the order
	double price; // Price at which the order is placed
	int quantity; // Quantity of the asset to be bought or sold
	std::chrono::steady_clock::time_point timestamp; // Timestamp of when the order was created

	Order(std::string id_, OrderSide side_, double price_, int quantity_, std::chrono::steady_clock::time_point ts = std::chrono::steady_clock::now())
		: id(std::move(id_)), side(side_), price(price_), quantity(quantity_), timestamp(ts) {
	}

};


struct BidComparator {
	bool operator()(const Order& a, const Order& b) const {
		return (a.price > b.price) || (a.price == b.price && a.timestamp < b.timestamp);
	}
};

struct AskComparator {
	bool operator()(const Order& a, const Order& b) const {
		return (a.price < b.price) || (a.price == b.price && a.timestamp < b.timestamp);
	}
};

