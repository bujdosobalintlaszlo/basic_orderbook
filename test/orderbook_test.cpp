#include<gtest/gtest.h>
#include "orderbook/orderbook.h"
#include "orderbook/ordertype.h"
#include "orderbook/order.h"
#include "orderbook/trade.h"
#include <gmock/gmock.h> 
using ::testing::AllOf;
using ::testing::Field;
using ::testing::ElementsAre;

class OrderBookTest : public ::testing::Test{
protected:
	 OrderBook ob;
	 //___ HELPERS FOR TESTING ___
	 bool checkTradeVals(Trades expected, Trades output) const{
		  if(expected.size() != output.size()) return false;
		  for(size_t i{0};i<expected.size();++i){
				if(expected[i].getBidTrade() != output[i].getBidTrade() || expected[i].getAskTrade() != output[i].getAskTrade()){
					 return false;
				}
		  }
		  return true;
	 }
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
	 EXPECT_EQ(ob.getAsks().size(),1);
	 EXPECT_EQ(ob.getBids().size(),0);
}

TEST_F(OrderBookTest,MarketOrderPartialFill){
    Order o(1, OrderType::PostOnly, Side::SELL, 67008000000, 7);
    Order o2(2,Side::BUY, 40);
    ob.placeOrder(std::make_unique<Order>(o));
    Trades trade = ob.placeOrder(std::make_unique<Order>(o2));
	 EXPECT_THAT(trade, ElementsAre(
        AllOf(
            Property(&Trade::getAskTrade, AllOf(
                Field(&TradeInfo::orderId_, 1),
                Field(&TradeInfo::quantity_,0)
            )),
            Property(&Trade::getBidTrade, AllOf(
                Field(&TradeInfo::orderId_, 2),
                Field(&TradeInfo::quantity_,33)
            ))
        )
    ));
	 EXPECT_EQ(ob.getAsks().size(),0);
	 EXPECT_EQ(ob.getBids().size(),0);
	 
}

TEST_F(OrderBookTest,MarketOrderCantFill){
    //Order o(1, OrderType::PostOnly, Side::SELL, 67008000000, 7);
    Order o2(2,Side::BUY, 40);
    //ob.placeOrder(std::make_unique<Order>(o));
    Trades trade = ob.placeOrder(std::make_unique<Order>(o2));
	 ASSERT_EQ(trade.size(),0);
	 EXPECT_EQ(ob.getAsks().size(),0);
	 EXPECT_EQ(ob.getBids().size(),0);
	 
}

TEST_F(OrderBookTest,FillAndKillOrderMatch){
	 Order o(1, OrderType::PostOnly, Side::SELL, 67008000000, 7);
    Order o2(2, OrderType::FillAndKill, Side::BUY, 67008000000, 7);
    ob.placeOrder(std::make_unique<Order>(o));
    Trades trade = ob.placeOrder(std::make_unique<Order>(o2));
	 EXPECT_THAT(trade, ElementsAre(
        AllOf(
            Property(&Trade::getAskTrade, AllOf(
                Field(&TradeInfo::orderId_, 1),
                Field(&TradeInfo::quantity_,0)
            )),
            Property(&Trade::getBidTrade, AllOf(
                Field(&TradeInfo::orderId_, 2),
                Field(&TradeInfo::quantity_,0)
            ))
        )
    ));
	 EXPECT_EQ(ob.getAsks().size(),0);
	 EXPECT_EQ(ob.getBids().size(),0);
}

TEST_F(OrderBookTest,FillAndKillCantMatch){
    Order o2(2, OrderType::FillAndKill, Side::BUY, 67008000000, 7);
    Trades trade = ob.placeOrder(std::make_unique<Order>(o2));
	 ASSERT_EQ(trade.size(),0); 
	 EXPECT_EQ(ob.getAsks().size(),0);
	 EXPECT_EQ(ob.getBids().size(),0);
}

TEST_F(OrderBookTest,FillAndKillPartialFill){
	 Order o(1, OrderType::PostOnly, Side::SELL, 67008000000, 6);
    Order o2(2, OrderType::FillAndKill, Side::BUY, 67008000000, 7);
    ob.placeOrder(std::make_unique<Order>(o));
    Trades trade = ob.placeOrder(std::make_unique<Order>(o2));
	 EXPECT_THAT(trade, ElementsAre(
        AllOf(
            Property(&Trade::getAskTrade, AllOf(
                Field(&TradeInfo::orderId_, 1),
                Field(&TradeInfo::quantity_,0)
            )),
            Property(&Trade::getBidTrade, AllOf(
                Field(&TradeInfo::orderId_, 2),
                Field(&TradeInfo::quantity_,1)
            ))
        )
    ));
	 EXPECT_EQ(ob.getAsks().size(),0);
	 EXPECT_EQ(ob.getBids().size(),0);
}

TEST_F(OrderBookTest,FillOrKillCanMatch){
	 Order o(1, OrderType::PostOnly, Side::SELL, 67008000000, 6);
    Order o2(2, OrderType::FillOrKill, Side::BUY, 67008000000, 6);
    ob.placeOrder(std::make_unique<Order>(o));
    Trades trade = ob.placeOrder(std::make_unique<Order>(o2));
	 EXPECT_THAT(trade, ElementsAre(
        AllOf(
            Property(&Trade::getAskTrade, AllOf(
                Field(&TradeInfo::orderId_, 1),
                Field(&TradeInfo::quantity_,0)
            )),
            Property(&Trade::getBidTrade, AllOf(
                Field(&TradeInfo::orderId_, 2),
                Field(&TradeInfo::quantity_,0)
            ))
        )
    ));
	 EXPECT_EQ(ob.getAsks().size(),0);
	 EXPECT_EQ(ob.getBids().size(),0);
}

TEST_F(OrderBookTest, FillOrKillCanMatchMultipleOrder) {
    Order o(1, OrderType::PostOnly, Side::SELL, 67008000000, 3);
    Order o3(3, OrderType::PostOnly, Side::SELL, 67008000000, 3);
    Order o2(2, OrderType::FillOrKill, Side::BUY, 67008000000, 6);

    ob.placeOrder(std::make_unique<Order>(o));
    ob.placeOrder(std::make_unique<Order>(o3));
    Trades trade = ob.placeOrder(std::make_unique<Order>(o2));
	 std::cout << "trade size: " << trade.size() << '\n'; 
    EXPECT_THAT(trade, ElementsAre(
        AllOf(
            Property(&Trade::getAskTrade, AllOf(
                Field(&TradeInfo::orderId_, 1),
                Field(&TradeInfo::quantity_, 0)
            )),
            Property(&Trade::getBidTrade, AllOf(
                Field(&TradeInfo::orderId_, 2),
                Field(&TradeInfo::quantity_, 3)
            ))
        ),
        AllOf(
            Property(&Trade::getAskTrade, AllOf(
                Field(&TradeInfo::orderId_, 3),
                Field(&TradeInfo::quantity_, 0)
            )),
            Property(&Trade::getBidTrade, AllOf(
                Field(&TradeInfo::orderId_, 2),
                Field(&TradeInfo::quantity_, 0)
            ))
        )
    ));

    EXPECT_EQ(ob.getAsks().size(), 0);
    EXPECT_EQ(ob.getBids().size(), 0);
}
TEST_F(OrderBookTest,FillOrKillCantMatch){
	 Order o(1, OrderType::PostOnly, Side::SELL, 67008000000, 3);
    Order o2(2, OrderType::FillOrKill, Side::BUY, 67008000000, 6);
    ob.placeOrder(std::make_unique<Order>(o));
    Trades trade = ob.placeOrder(std::make_unique<Order>(o2));
	 ASSERT_EQ(trade.size(),0); 
	 EXPECT_EQ(ob.getAsks().size(),1);
	 EXPECT_EQ(ob.getBids().size(),0);
}

TEST_F(OrderBookTest,GoodTillCancelInsert){

}

TEST_F(OrderBookTest,GoodTillCancelPartialFill){

}

TEST_F(OrderBookTest,GoodTillCancellFullyFilled){

}
