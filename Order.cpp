#include "Order.h"

Order::Order(int id, OrderSide side, double price, int quantity)
   : id(id), side(side), price(price), quantity(quantity) {
   timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
       std::chrono::system_clock::now().time_since_epoch()).count();
}

int Order::getID() const {return id;}
OrderSide Order::getSide() const { return side; }   
double Order::getPrice() const { return price; }
int Order::getQuantity() const { return quantity; }
long long Order::getTimestamp() const { return timestamp; }


