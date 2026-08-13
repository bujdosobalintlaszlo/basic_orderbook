#include<memory>
#include "orderbook/order.h"
#include "orderbook/orderbook.h"

int main(){
	 Order o(1,OrderType::PostOnly,Side::BUY,8392132,1);
	 Order o2(2,OrderType::PostOnly,Side::SELL,6767676767,67);
	 Order o3(3,OrderType::FillOrKill,Side::BUY,6767676767,67);
	 OrderBook ob{};
	 ob.placeOrder(std::make_unique<Order>(o));
	 ob.placeOrder(std::make_unique<Order>(o2));
	 ob.displayBids();
	 ob.displayAsks();
	 //ob.placeOrder(std::make_unique<Order>(o3));
	 //ob.placeOrder(std::make_unique<Order>(o3));
	 ob.displayBids();
	 ob.displayAsks();
	 ob.cancelOrder(1);
	 ob.displayBids();
	 ob.displayAsks();
	 ob.cancelOrder(1);
	 ob.displayBids();
	 ob.displayAsks();
}
