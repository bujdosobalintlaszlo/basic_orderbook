#include<gtest/gtest.h>
#include "orderbook/orderbook.h"
/*

*/
class OrderBookTest : public ::testing::Test{
protected:
	 OrderBook ob;
};
TEST_F(OrderBookTest,CreatingBookWithOrders){
	 Order o(1,OrderType::PostOnly,Side::BUY,6700000,67);
	 Trades trades = ob.placeOrder(std::make_unique<Order>(o));
	 ASSERT_EQ(trades.size(),0);
	 ASSERT_EQ(ob.getBids().size(),1);
}

TEST_F(OrderBookTest,AdddingRedundantIdsToTheBook){
	 Order o1(1,OrderType::PostOnly,Side::BUY,6700000,67);
	 Order o2(1,OrderType::PostOnly,Side::BUY,6700000,67);
	 Trades trades1 = ob.placeOrder(std::make_unique<Order>(o1));
	 Trades trades2 = ob.placeOrder(std::make_unique<Order>(o2));
	 ASSERT_EQ(trades1.size(),0);
	 ASSERT_EQ(trades2.size(),0);
	 EXPECT_EQ(ob.getBids().size(),1);
}

TEST_F(OrderBookTest,CancelValidOrder){
	 Order o(1,OrderType::PostOnly,Side::BUY,6700000,67);
	 //no needs to store trades
	 ob.placeOrder(std::make_unique<Order>(o));
	 bool succesfullDel = ob.cancelOrder(1);
	 ASSERT_TRUE(succesfullDel);
	 EXPECT_EQ(ob.getBids().size(),0);
}

TEST_F(OrderBookTest,CancelInvalidOrder){
	 Order o(1,OrderType::PostOnly,Side::BUY,6700000,67);
	 //no need to store trades
	 ob.placeOrder(std::make_unique<Order>(o));
	 bool succesfulDel = ob.cancelOrder(2);
	 ASSERT_FALSE(succesfulDel);
	 EXPECT_EQ(ob.getBids().size(),1);
}

TEST_F(OrderBookTest,LimitOrderTest){
	 Order o(1,OrderType::PostOnly,Side::SELL,6700800,67);
	 Order o2(2,OrderType::Limit,Side::BUY,670000,67);
	 ob.placeOrder(std::make_unique<Order>(o));
	 Trades trade = ob.placeOrder(std::make_unique<Order>(o2));
	 ASSERT_EQ(trade.size(),1);
}

TEST_F(OrderBookTest,LimitOrderFail){
	 Order o(1,OrderType::PostOnly,Side::SELL,6700800,67);
	 Order o2(2,OrderType::Limit,Side::BUY,670000,67);
}

TEST_F(OrderBookTest,MarketOrderFill){
	 
}

TEST_F(OrderBookTest,MarketOrderCantMatch){

}

TEST_F(OrderBookTest,FillAndKillOrderMatch){

}

TEST_F(OrderBookTest,FillAndKillCantMatch){

}

TEST_F(OrderBookTest,FillAndKillPartialFill){

}

TEST_F(OrderBookTest,FillOrKillCanMatch){

}

TEST_F(OrderBookTest,FillOrKillCantMatch){

}

TEST_F(OrderBookTest,GoodTillCancelInsert){

}

TEST_F(OrderBookTest,GoodTillCancelPartialFill){

}

TEST_F(OrderBookTest,GoodTillCancellFullyFilled){

}
