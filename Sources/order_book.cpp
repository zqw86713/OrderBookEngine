#include "../Headers/order_book.h"
#include<algorithm>
#include <iostream>


void OrderBook::addOrder(const Order& order) {
  if (order.getSide() == OrderSide::BUY)
    buy_orders_.push(order);
  else
    sell_orders_.push(order);
}

void OrderBook::matchOrders() {
  while (!buy_orders_.empty() && !sell_orders_.empty()) {
    const Order& buy = buy_orders_.top();
    const Order& sell = sell_orders_.top();

    if (buy.getPrice() >= sell.getPrice()) {
      int match_qty = std::min(buy.getQuantity(), sell.getQuantity());
      std::cout << "Matched: " << match_qty << " units @ " << sell.getPrice()
                << "\n";

      buy_orders_.pop();
      sell_orders_.pop();

      int buy_remain = buy.getQuantity() - match_qty;
      int sell_remain = sell.getQuantity() - match_qty;

      //buy.quantity -= match_qty;
      //sell.quantity -= match_qty;



      if (buy_remain > 0) {
      
        Order remaining_buy(buy.getID(), buy.getSide(), buy.getPrice(),
                            buy_remain);

        buy_orders_.push(remaining_buy);
      }

      if (sell_remain > 0) {
        Order remaining_sell(sell.getID(), sell.getSide(), sell.getPrice(), sell_remain);
        sell_orders_.push(remaining_sell);
      }


    } else {
      break;    // No further matches possible.
    }
  }
}

void OrderBook::cancelOrder(int orderID) { 
    std::priority_queue<Order> new_buy;

    while (buy_orders_.empty()) {
      Order current = buy_orders_.top();
      buy_orders_.pop();
      if (current.getID() != orderID) {
        new_buy.push(current);
      }
    }
    buy_orders_ = std::move(new_buy);

    std::priority_queue<Order> new_sell;

    while (!sell_orders_.empty()) {
      Order current = sell_orders_.top();
      sell_orders_.pop();
      if (current.getID() != orderID) {
        new_sell.push(current);
      }
    
    }
    sell_orders_ = std::move(new_sell);

}