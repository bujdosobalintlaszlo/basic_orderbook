#include<gtest/gtest.h>
#include "orderbook/orderbook.h"
#include<memory>
TEST(OrderBookTestSuite,CreatingBookWithOrders){
	 Order o(1,OrderType::PostOnly,Side::BUY,6700000,67);
	 OrderBook ob{};
	 ob.placeOrder(std::make_unique<Order>(o));
	 EXPECT_EQ(ob.getBids().size(),1);

}

TEST(OrderBookTestSuite,AdddingRedundantIdsToTheBook){
	 Order o1(1,OrderType::PostOnly,Side::BUY,6700000,67);
	 Order o2(1,OrderType::PostOnly,Side::BUY,6700000,67);
	 OrderBook ob{};
	 ob.placeOrder(std::make_unique<Order>(o1));
	 ob.placeOrder(std::make_unique<Order>(o2));
	 EXPECT_EQ(ob.getBids().size(),1);
}

TEST(OrderBookTestSuite,CancelValidOrder){
	 Order o(1,OrderType::PostOnly,Side::BUY,6700000,67);
	 OrderBook ob{};
	 ob.placeOrder(std::make_unique<Order>(o));
	 ob.cancelOrder(1);
	 EXPECT_EQ(ob.getBids().size(),0);
}

TEST(OrderBookTestSuite,CancelInvalidOrder){
	 Order o(1,OrderType::PostOnly,Side::BUY,6700000,67);
	 OrderBook ob{};
	 ob.placeOrder(std::make_unique<Order>(o));
	 ob.cancelOrder(2);
	 EXPECT_EQ(ob.getBids().size(),1);
}

TEST(OrderBookTestSuite,LimitOrderTest){
	 Order o(1,OrderType::PostOnly,Side::BUY,670000,67);
	 Order o2(2,OrderType::Limit,Side::BUY,670000,67);
}
