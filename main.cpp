#include <iostream>
#include "order.h"
#include "order_book.h"

int main() {
	//Day 1 test.
	//Order order1(1, OrderSide::BUY, 100.5, 10);
	//std::cout << "Order ID: " << order1.getID() << std::endl;
	//std::cout << "Price: " << order1.getPrice() << std::endl;
	//std::cout << "Quantity: " << order1.getQuantity() << std::endl;
	//std::cout << "Timestamp: " << order1.getTimestamp() << std::endl;

	//Day 2 test.
	//OrderBook ob;

	////seed some order
	 //       ob.addOrder(Order(1, OrderSide ::BUY, 101.0, 10));
	 //       ob.addOrder(Order(2, OrderSide ::SELL, 100.0, 5));
	 //       ob.addOrder(Order(3, OrderSide::SELL, 99.5, 7));

	//std::cout << "Matching orders...\n";
 //       ob.matchOrders();
 //       std::cout << "Done.\n";
        OrderBook ob;
        ob.addOrder(Order(1, OrderSide::BUY, 101.0, 100));
        ob.addOrder(Order(2, OrderSide::SELL, 102.0, 50));
        ob.cancelOrder(1);
        ob.matchOrders();
        std::cout << "Testing cancelOrder():\n";
        std::cout << "Expect no matching since BUY order 1 was cancelled.\n";


	return 0;

}