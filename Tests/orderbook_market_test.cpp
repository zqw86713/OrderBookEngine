// Tests/orderbook_market_test.cpp

#include <gtest/gtest.h>

#include "OrderBook.h"

// Market Buy should match against best Ask
TEST(MarketOrderTest, BuyMarketMatchesBestAsk) {
  OrderBook ob;
  // Add a limit sell order
  ob.addOrder(Order("s1", OrderSide::SELL, 100.0, 5));
  // Add a market buy order
  ob.addOrder(Order("m1", OrderSide::BUY, 0.0, 8, OrderType::MARKET));
  ob.matchOrders();

  auto trades = ob.getExecutedTrades();
  ASSERT_EQ(trades.size(), 1);
  EXPECT_EQ(trades[0].buy_order_id, "m1");
  EXPECT_EQ(trades[0].sell_order_id, "s1");
  EXPECT_EQ(trades[0].quantity, 5);
  EXPECT_DOUBLE_EQ(trades[0].price, 100.0);

  // Order book should now be empty
  EXPECT_FALSE(ob.getTopBid().has_value());
  EXPECT_FALSE(ob.getTopAsk().has_value());
}

// Market Sell should match against best Bid
TEST(MarketOrderTest, SellMarketMatchesBestBid) {
  OrderBook ob;
  ob.addOrder(Order("b1", OrderSide::BUY, 95.0, 10));
  ob.addOrder(Order("m2", OrderSide::SELL, 0.0, 6, OrderType::MARKET));
  ob.matchOrders();

  auto trades = ob.getExecutedTrades();
  ASSERT_EQ(trades.size(), 1);
  EXPECT_EQ(trades[0].buy_order_id, "b1");
  EXPECT_EQ(trades[0].sell_order_id, "m2");
  EXPECT_EQ(trades[0].quantity, 6);
  EXPECT_DOUBLE_EQ(trades[0].price, 95.0);
}

// Market Partial Fill should leave remaining limit order
TEST(MarketOrderTest, MarketPartialFillNoReinsert) {
  OrderBook ob;
  ob.addOrder(Order("s2", OrderSide::SELL, 50.0, 10));
  ob.addOrder(Order("m3", OrderSide::BUY, 0.0, 4, OrderType::MARKET));
  ob.matchOrders();

  auto trades = ob.getExecutedTrades();
  ASSERT_EQ(trades.size(), 1);
  EXPECT_EQ(trades[0].quantity, 4);

  auto topAsk = ob.getTopAsk();
  ASSERT_TRUE(topAsk.has_value());
  EXPECT_EQ(topAsk->id, "s2");
  EXPECT_EQ(topAsk->quantity, 6);
}

// A lone market order with no counterparty should produce no trade
TEST(MarketOrderTest, MarketAloneNoTrade) {
  OrderBook ob;
  ob.addOrder(Order("m4", OrderSide::BUY, 0.0, 5, OrderType::MARKET));
  ob.matchOrders();

  auto trades = ob.getExecutedTrades();
  EXPECT_TRUE(trades.empty());
}
