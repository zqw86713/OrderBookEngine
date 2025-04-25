#include "order.h"

Order::Order(int id, OrderSide side, double price, int quantity)
   : id(id), side(side), price(price), quantity(quantity) {
   timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
       std::chrono::system_clock::now().time_since_epoch()).count();
}


bool Order::operator<(const Order& other) const {
  if (price == other.price) {
    return timestamp > other.timestamp;
  }
  if (side == OrderSide::BUY) {
    return price < other.price;
  }
  return price > other.price;
}