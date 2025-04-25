#pragma once
#include <string>
#include <chrono>

// This is the code that written yesterday.
enum class OrderSide {
	BUY,
	SELL
};


class Order {
public:
    Order(int id, OrderSide side, double price, int quantity);


		int getID() const { return id; }
        OrderSide getSide() const { return side; }
        double getPrice() const { return price; }
        int getQuantity() const { return quantity; }
        long long getTimestamp() const { return timestamp; }

		bool operator<(const Order& other) const;

private:
	int id;
	OrderSide side;
	double price;
	int quantity;
	long long timestamp; // Timestamp in milliseconds since epoch


};

