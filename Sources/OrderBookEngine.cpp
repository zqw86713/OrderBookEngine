// OrderBookEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include <iostream>

//int main()
//{
//    std::cout << "Hello World!\n";
//}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


#include <iostream>
#include "../Headers/OrderBookEngine.h"
#include <algorithm>


void OrderBookEngine::submitOrder(const Order& incomingOrder) {
  std::lock_guard<std::mutex> lock(bookMutex);

  // Make a copy so we can modify the quantity during matching
  Order order = incomingOrder;
  matchOrders(order);

  // If not fully filled, add to book and track in ID map
  if (order.getQuantity() > 0) {
    auto& book = (order.getSide() == OrderSide::BUY) ? buyBook : sellBook;
    auto& queue = book[order.getPrice()];
    queue.push_back(order);

    OrderRef ref = std::prev(queue.end());


    orderIdMap[order.getID()] =
        OrderLocation{order.getPrice(), order.getSide(), ref};
  }
}



void OrderBookEngine::matchOrders(Order order) {
  auto& book = (order.getSide() == OrderSide::BUY) ? sellBook : buyBook;

  while (order.getQuantity() > 0 && !book.empty()) {
    auto it = (order.getSide() == OrderSide::BUY) ? book.begin()
                                                  : std::prev(book.end());

    // Check price condition (crossing)
    if ((order.getSide() == OrderSide::BUY && order.getPrice() < it->first) ||
        (order.getSide() == OrderSide::SELL && order.getPrice() > it->first)) {
      break;
    }

    auto& queue = it->second;
    while (!queue.empty() && order.getQuantity() > 0) {
      Order& restingOrder = queue.front();

      int matchQty = std::min(order.getQuantity(), restingOrder.getQuantity());

      // Reduce both orders
      order.reduceQuantity(matchQty);
      restingOrder.reduceQuantity(matchQty);

      std::cout << "Matched " << matchQty << " @ $" << it->first
                << " (Order ID: " << restingOrder.getID() << ")\n";

      if (restingOrder.getQuantity() == 0) {
        orderIdMap.erase(restingOrder.getID());
        queue.pop_front();
      }
    }

    if (queue.empty()) {
      book.erase(it);
    }
  }
}



bool OrderBookEngine::cancelOrder(int orderId) {
  std::lock_guard<std::mutex> lock(bookMutex);

  auto it = orderIdMap.find(orderId);
  if (it == orderIdMap.end()) return false;

  const OrderLocation& loc = it->second;
  auto& book = (loc.side == OrderSide::BUY) ? buyBook : sellBook;

  auto levelIt = book.find(loc.price);
  if (levelIt == book.end()) return false;

  auto& queue = levelIt->second;
  queue.erase(loc.ref);

  if (queue.empty()) {
    book.erase(levelIt);
  }

  orderIdMap.erase(it);
  std::cout << "Canceled order ID: " << orderId << "\n";
  return true;
}