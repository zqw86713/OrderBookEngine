#include <gtest/gtest.h>

#include "OrderBook.h"

// Test limit buy matching against best ask
TEST(LimitOrderTest, BuyLimitMatchesBestAsk) {
  OrderBook ob;
  // Add a limit sell order at price 100, quantity 5
  ob.addOrder(Order("s1", OrderSide::SELL, 100.0, 5, OrderType::LIMIT));
  // Add a limit buy order at price 105, quantity 3
  ob.addOrder(Order("b1", OrderSide::BUY, 105.0, 3, OrderType::LIMIT));
  ob.matchOrders();

  auto trades = ob.getExecutedTrades();
  ASSERT_EQ(trades.size(), 1);
  EXPECT_EQ(trades[0].buy_order_id, "b1");
  EXPECT_EQ(trades[0].sell_order_id, "s1");
  EXPECT_EQ(trades[0].quantity, 3);
  EXPECT_DOUBLE_EQ(trades[0].price, 100.0);

  // Remaining ask quantity should be 2
  auto topAsk = ob.getTopAsk();
  ASSERT_TRUE(topAsk.has_value());
  EXPECT_EQ(topAsk->id, "s1");
  EXPECT_EQ(topAsk->quantity, 2);
  // No bid remains
  EXPECT_FALSE(ob.getTopBid().has_value());
}

// Test limit sell matching against best bid
TEST(LimitOrderTest, SellLimitMatchesBestBid) {
  OrderBook ob;
  // Add a limit buy order at price 120, quantity 5
  ob.addOrder(Order("b1", OrderSide::BUY, 120.0, 5, OrderType::LIMIT));
  // Add a limit sell order at price 115, quantity 2
  ob.addOrder(Order("s1", OrderSide::SELL, 115.0, 2, OrderType::LIMIT));
  ob.matchOrders();

  auto trades = ob.getExecutedTrades();
  ASSERT_EQ(trades.size(), 1);
  EXPECT_EQ(trades[0].buy_order_id, "b1");
  EXPECT_EQ(trades[0].sell_order_id, "s1");
  EXPECT_EQ(trades[0].quantity, 2);
  EXPECT_DOUBLE_EQ(trades[0].price, 115.0);

  // Remaining bid quantity should be 3
  auto topBid = ob.getTopBid();
  ASSERT_TRUE(topBid.has_value());
  EXPECT_EQ(topBid->id, "b1");
  EXPECT_EQ(topBid->quantity, 3);
  // No ask remains
  EXPECT_FALSE(ob.getTopAsk().has_value());
}

// Test no match when prices do not cross
TEST(LimitOrderTest, NoMatchWhenPricesDoNotCross) {
  OrderBook ob;
  ob.addOrder(Order("b1", OrderSide::BUY, 90.0, 5, OrderType::LIMIT));
  ob.addOrder(Order("s1", OrderSide::SELL, 100.0, 5, OrderType::LIMIT));
  ob.matchOrders();

  // No trades executed
  auto trades = ob.getExecutedTrades();
  EXPECT_TRUE(trades.empty());

  // Both orders remain
  auto topBid = ob.getTopBid();
  ASSERT_TRUE(topBid.has_value());
  EXPECT_EQ(topBid->id, "b1");
  auto topAsk = ob.getTopAsk();
  ASSERT_TRUE(topAsk.has_value());
  EXPECT_EQ(topAsk->id, "s1");
}

// Test partial fill leaves remaining limit order
TEST(LimitOrderTest, PartialFillLeavesRemaining) {
  OrderBook ob;
  ob.addOrder(Order("b1", OrderSide::BUY, 100.0, 5, OrderType::LIMIT));
  ob.addOrder(Order("s1", OrderSide::SELL, 100.0, 3, OrderType::LIMIT));
  ob.matchOrders();

  auto trades = ob.getExecutedTrades();
  ASSERT_EQ(trades.size(), 1);
  EXPECT_EQ(trades[0].quantity, 3);

  // Remaining bid quantity should be 2
  auto topBid = ob.getTopBid();
  ASSERT_TRUE(topBid.has_value());
  EXPECT_EQ(topBid->id, "b1");
  EXPECT_EQ(topBid->quantity, 2);
  // No ask remains
  EXPECT_FALSE(ob.getTopAsk().has_value());
}

// Test canceling a limit order removes it from the book
TEST(LimitOrderTest, CancelOrderRemovesLimit) {
  OrderBook ob;
  ob.addOrder(Order("b1", OrderSide::BUY, 100.0, 5, OrderType::LIMIT));
  auto topBid = ob.getTopBid();
  ASSERT_TRUE(topBid.has_value());
  EXPECT_EQ(topBid->id, "b1");

  // Cancel and verify removal
  EXPECT_TRUE(ob.cancelOrder("b1"));
  EXPECT_FALSE(ob.getTopBid().has_value());
}

// Test modifying a limit order updates price and quantity
TEST(LimitOrderTest, ModifyOrderUpdatesLimit) {
  OrderBook ob;
  ob.addOrder(Order("b1", OrderSide::BUY, 100.0, 5, OrderType::LIMIT));
  EXPECT_TRUE(ob.modifyOrder("b1", 110.0, 2));

  auto topBid = ob.getTopBid();
  ASSERT_TRUE(topBid.has_value());
  EXPECT_EQ(topBid->id, "b1");
  EXPECT_EQ(topBid->quantity, 2);
  EXPECT_DOUBLE_EQ(topBid->price, 110.0);
}
