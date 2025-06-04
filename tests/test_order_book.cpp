// tests/test_order_book.cpp

#include <gtest/gtest.h>

#include "order_book.h"

// Capture printed output for verification.
#include <iostream>
#include <sstream>

class OrderBookTest : public ::testing::Test {
 protected:
  void captureStdout() { old_buf_ = std::cout.rdbuf(buf_.rdbuf()); }
  std::string releaseStdout() {
    std::cout.rdbuf(old_buf_);
    return buf_.str();
  }

  std::stringstream buf_;
  std::streambuf* old_buf_;
};

TEST_F(OrderBookTest, SimpleMatch) {
  captureStdout();
  OrderBook ob;
  ob.addOrder(Order(1, OrderSide::BUY, 101.0, 10));
  ob.addOrder(Order(2, OrderSide::SELL, 100.0, 5));
  ob.matchOrders();

  std::string output = releaseStdout();
  EXPECT_NE(output.find("Matched: 5 units @ 100"), std::string::npos);
}

TEST_F(OrderBookTest, PartialMatchAndRemainder) {
  captureStdout();
  OrderBook ob;
  ob.addOrder(Order(1, OrderSide::BUY, 50.0, 3));
  ob.addOrder(Order(2, OrderSide::SELL, 45.0, 5));
  ob.matchOrders();

  std::string output = releaseStdout();
  // Should match only 3 units, leaving 2 units on the sell side.
  EXPECT_NE(output.find("Matched: 3 units @ 45"), std::string::npos);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
