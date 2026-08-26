#include<memory>
#include "orderbook/order.h"
#include "orderbook/orderbook.h"
#include "orderbook/trade_info.h"
#include "orderbook/trade.h"

int main(){
	 Order o(1,OrderType::PostOnly,Side::BUY,100000000000000000,1);
	 Order o2(2,OrderType::PostOnly,Side::BUY,6767676767,66);
	 Order o3(3,OrderType::FillOrKill,Side::SELL,6767676767,67);
	 OrderBook ob{};
	 ob.placeOrder(std::make_unique<Order>(o));
	 ob.placeOrder(std::make_unique<Order>(o2));
	 ob.displayBids();
	 ob.displayAsks();
	 std::cout << "_____Trades_____" << '\n';
	 Trades trades = ob.placeOrder(std::make_unique<Order>(o3));
	 for(Trade t: trades){
		  std::cout << t << '\n';
	 }
	 ob.placeOrder(std::make_unique<Order>(o3));
	 ob.displayBids();
	 ob.displayAsks();
}
