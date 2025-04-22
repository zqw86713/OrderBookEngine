#include <iostream>
#include "Order.h"

int main() {
	Order order1(1, OrderSide::BUY, 100.5, 10);
	std::cout << "Order ID: " << order1.getID() << std::endl;
	std::cout << "Price: " << order1.getPrice() << std::endl;
	std::cout << "Quantity: " << order1.getQuantity() << std::endl;
	std::cout << "Timestamp: " << order1.getTimestamp() << std::endl;

	return 0;

}