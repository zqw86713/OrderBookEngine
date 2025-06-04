// OrderBook.h
#pragma once

#include <string>
#include <chrono>
#include <unordered_map>
#include <set>
#include <optional>

enum class OrderSide { BUY, SELL };

struct Order {
    std::string id;
    OrderSide side;
    double price;
    int quantity;
    std::chrono::steady_clock::time_point timestamp;

    Order(std::string id_, OrderSide side_, double price_, int quantity_,
        std::chrono::steady_clock::time_point ts = std::chrono::steady_clock::now())
        : id(std::move(id_)), side(side_), price(price_), quantity(quantity_), timestamp(ts) {
    }

    bool operator==(const Order& other) const {
        return id == other.id;
    }

    void reduceQuantity(int amount) {
        quantity -= amount;
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

class OrderBook {
public:
    bool addOrder(const Order& order);
    bool cancelOrder(const std::string& order_id);
    bool modifyOrder(const std::string& order_id, double new_price, int new_quantity);
    void matchOrders();

    std::optional<Order> getTopBid() const;
    std::optional<Order> getTopAsk() const;

private:
    std::unordered_map<std::string, std::set<Order>::iterator> order_lookup_;
    std::set<Order, BidComparator> buy_orders_;
    std::set<Order, AskComparator> sell_orders_;
};
