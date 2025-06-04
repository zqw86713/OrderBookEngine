
#pragma once  

#ifndef ORDER_BOOK_ENGINE_H
#define ORDER_BOOK_ENGINE_H

#include <deque>
#include <map>
#include <mutex>
#include <unordered_map>

#include "Order.h"  // Uses your existing Order class and OrderSide enum

using OrderRef = std::deque<Order>::iterator;

struct OrderLocation {
  double price;
  OrderSide side;
  OrderRef ref;
};

class OrderBookEngine {
 public:
  OrderBookEngine() = default;

  void submitOrder(const Order& order);
  bool cancelOrder(int orderId);

 private:
  std::map<double, std::deque<Order>> buyBook;
  std::map<double, std::deque<Order>> sellBook;

  std::unordered_map<int, OrderLocation> orderIdMap;
  std::mutex bookMutex;

  void matchOrders(Order order);
};

#endif  // ORDER_BOOK_ENGINE_H
