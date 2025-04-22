#pragma once
#include <string>
#include <chrono>

enum class OrderSide {
	BUY,
	SELL
};	


class Order {
public:
	Order(int id, OrderSide side, double price, int quantity);

	int getID() const;
	OrderSide getSide() const;
	double getPrice() const;
	int getQuantity() const;
	long long getTimestamp() const;

private:
	int id;
	OrderSide side;
	double price;
	int quantity;
	long long timestamp; // Timestamp in milliseconds since epoch


};
