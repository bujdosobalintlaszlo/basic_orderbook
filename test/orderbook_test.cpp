#include<gtest/gtest.h>
#include "orderbook/orderbook.h"
#include "orderbook/ordertype.h"
#include "orderbook/order.h"
#include "orderbook/trade.h"
#include <gmock/gmock.h> 
using ::testing::AllOf;
using ::testing::Field;
using ::testing::ElementsAre;
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
	 Order o(1,OrderType::PostOnly,Side::SELL,66000,67);
	 Order o2(2,OrderType::Limit,Side::BUY,670000,67);
	 ob.placeOrder(std::make_unique<Order>(o));
	 Trades trade = ob.placeOrder(std::make_unique<Order>(o2));
	 std::cout << trade.size() << '\n';
	 ASSERT_EQ(trade.size(),1);
}

TEST_F(OrderBookTest,LimitOrderFail){
	 Order o(1,OrderType::PostOnly,Side::SELL,6700800,67);
	 Order o2(2,OrderType::Limit,Side::BUY,670000,67);
	 ob.placeOrder(std::make_unique<Order>(o));
	 Trades trade = ob.placeOrder(std::make_unique<Order>(o2));
	 ASSERT_EQ(trade.size(),0);
}

TEST_F(OrderBookTest, LimitOrderPartialFill){
    Order o(1, OrderType::PostOnly, Side::SELL, 6700800, 67);
    Order o2(2, OrderType::Limit, Side::BUY, 6700800, 40);
    ob.placeOrder(std::make_unique<Order>(o));
    Trades trade = ob.placeOrder(std::make_unique<Order>(o2));
    EXPECT_THAT(trade, ElementsAre(
        AllOf(
            Property(&Trade::getAskTrade, AllOf(
                Field(&TradeInfo::orderId_, 1),
                Field(&TradeInfo::quantity_,27)
            )),
            Property(&Trade::getBidTrade, AllOf(
                Field(&TradeInfo::orderId_, 2),
                Field(&TradeInfo::quantity_, 0)
            ))
        )
    ));
}

TEST_F(OrderBookTest,MarketOrderMatch){
    Order o(1, OrderType::PostOnly, Side::SELL, 67008000000, 67);
    Order o2(2,Side::BUY, 40);
    ob.placeOrder(std::make_unique<Order>(o));
    Trades trade = ob.placeOrder(std::make_unique<Order>(o2));
	 std::cout << "BID TRADE: " << trade[0].getBidTrade().orderId_ << " initial quantity: " << trade[0].getBidTrade().initialQuantity_ << " quanity " << trade[0].getBidTrade().quantity_ << " price "<<trade[0].getBidTrade().price_ << " side "<< trade[0].getBidTrade().side_<< '\n' << "ASK TRADE: " << trade[0].getAskTrade().orderId_ << " initial quantity: " << trade[0].getAskTrade().initialQuantity_ << " quanity " << trade[0].getAskTrade().quantity_ << " price "<<trade[0].getAskTrade().price_ << " side "<< trade[0].getAskTrade().side_ << '\n';
	 EXPECT_THAT(trade, ElementsAre(
        AllOf(
            Property(&Trade::getAskTrade, AllOf(
                Field(&TradeInfo::orderId_, 1),
                Field(&TradeInfo::quantity_,27)
            )),
            Property(&Trade::getBidTrade, AllOf(
                Field(&TradeInfo::orderId_, 2),
                Field(&TradeInfo::quantity_, 0)
            ))
        )
    ));
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
