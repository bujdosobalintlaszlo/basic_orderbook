#include "order.h"
#include "orderbook.h"
#include "side.h"
#include<string> 
#include<memory>
#include "ordertype.h"
#include "trade_info.h"
#include "orderLoaction.h"
class DebugOb{
private:
	 std::map<uint64_t,Orders,std::greater<int64_t>> bids_;
	 std::map<uint64_t,Orders> asks_;
	 std::unordered_map<uint64_t,OrderLocation> orders_;
	 
	 ///<summary>
	 ///params: the orders pointer, and bids_ or asks_ as a refference depending on BUY or SELL side order.
	 ///return: A vector of trades which went trough. Could be empty, which means it couldn't macth.
	 ///</summary>
	 template <typename Compare>
	 Trades matchMarketOrder(OrderPtr &order, std::map<Price,Orders, Compare> &book) {
		  Trades trades{};
		  auto it = book.begin();
		  while (it != book.end() && order->getRemainingQuantity() > 0) {
				auto &orders = it->second;
				auto orders_it = orders.begin();
				while (orders_it != orders.end() && order->getRemainingQuantity() > 0) {
					 auto &current_order = *orders_it;
					 uint64_t fill_qty{0};
					 if(order->getSide() == Side::BUY && order->getPrice() >= current_order->getPrice()){
						  fill_qty = std::min(order->getRemainingQuantity(), current_order->getRemainingQuantity());
					 }
					 else if(order->getSide() == Side::SELL && order->getPrice() <= current_order->getPrice()){
						  fill_qty = std::max(order->getRemainingQuantity(), current_order->getRemainingQuantity());
					 }
					 if(fill_qty > 0){
						  uint64_t fill_price = current_order->getPrice();
						  order->fill(fill_qty);
						  current_order->fill(fill_qty);
						  trades.push_back(createTradeData(order, current_order));
					 }
					 //if the current order from the book got fully filled, then we remove it from the list, storing trades at the given price level
					 if (current_order->getRemainingQuantity() == 0) {
						  orders_it = orders.erase(orders_it);
					 } else {
						  ++orders_it;
					 }
				}
				//checking if the price level got emptyed. If so we delete it from the book.
				if (orders.empty()) {
					 it = book.erase(it);
				} else {
					 ++it;
				}
		  }
		  return trades;
	 }
	 ///<summary> 
	 ///Creates a Trade obj which consists of 2 TradeInfo objects. Those can be created by the orders traits;
	 ///<summary>
	 Trade createTradeData(OrderPtr &bidOrder,OrderPtr &askOrder) const{
		  	TradeInfo bidInf(bidOrder->getId(),bidOrder->getPrice(),bidOrder->getSide(),bidOrder->getRemainingQuantity(),bidOrder->getInitialQuantity());
		   TradeInfo askInf(askOrder->getId(),askOrder->getPrice(),askOrder->getSide(),askOrder->getRemainingQuantity(),askOrder->getInitialQuantity());
		   Trade trade(bidInf,askInf);
			return trade;
	 }

	 template<typename Comparator>
	 bool insertIntoBook(OrderPtr &order,std::map<Price,Orders,Comparator> &book){
		  const uint64_t price = order->getPrice();
		  auto [it, inserted] = book.try_emplace(price);
		  it->second.push_back(std::move(order));
		  return true;
	 }
	 
	 template<typename Comparator>
	 void cancelOrder(uint64_t id){
		  
	 }
	 template<typename Comparator>
	 Trade FOK(OrderPtr &order,std::map<Price,Orders,Comparator> &book){
	    Trades trades{};
		 auto it = book.begin();
		 while(it != book.end()){
				auto &orders = it->second;
				auto orders_it = orders.begin();
				while(orders_it != orders.end()){
					 auto &current_order = *orders_it;
					 if(current_order->getInitialQuantity() == order->getInitialQuantity()){
						  uint64_t fill_qty{0};
						  if(order->getSide() == Side::BUY && order->getPrice() >= current_order->getPrice()){
								fill_qty = std::min(order->getRemainingQuantity(), current_order->getRemainingQuantity());
						  }
						  else if(order->getSide() == Side::SELL && order->getPrice() <= current_order->getPrice()){
								fill_qty = std::max(order->getRemainingQuantity(), current_order->getRemainingQuantity());
						  }
						  if(fill_qty > 0){
								uint64_t fill_price = current_order->getPrice();
								order->fill(fill_qty);
								current_order->fill(fill_qty);
								trades.push_back(createTradeData(order, current_order));
						  }
						  //if the current order from the book got fully filled, then we remove it from the list, storing trades at the given price level
						  if (current_order->getRemainingQuantity() == 0) {
								orders_it = orders.erase(orders_it);
						  } else {
								++orders_it;
						  } 
					 }
				}
		 }
		 return trades;
	 }
public:
	 DebugOb(){}
	 bool placeOrder(OrderPtr &order){
		  switch(order->getOrderType()){
		  case OrderType::GoodForDay:
				break;		
		  case OrderType::GoodTillCancel:
				if(order->getSide() == Side::BUY){
					 matchMarketOrder(order,asks_);
					 return insertIntoBook(order,bids_);
				}else{
					 matchMarketOrder(order,bids_);
					 return insertIntoBook(order,asks_);
				}
				break;
		  //done 
		  case OrderType::PostOnly:
				if(order->getSide() == Side::BUY){
					 return insertIntoBook(order,bids_);
				}else{

					 return insertIntoBook(order,asks_);
				}
				break;
		  
		  case OrderType::IceBerg:
				break;
		  //not in book
		  case OrderType::Market:
				if(order->getSide() == Side::BUY){
					 matchMarketOrder(order,asks_);
				}
				else{
					 matchMarketOrder(order,bids_);
				}
				std::cout << "A MARKET Order has been filled with Order ID: " << order->getId() << ", with " << order->getRemainingQuantity() << " remaining unfilled." << '\n';
				break;
		  case OrderType::FillAndKill:
				break;
		  case OrderType::FillOrKill:
				break;
	 }
	 return true;
}

	

};

int main(){
	 DebugOb ob{};
	 Order o(1,OrderType::PostOnly,Side::SELL,83921.32,1);
	 Order o2(2,OrderType::Market,Side::BUY,83921.32,1);
	 std::unique_ptr<Order> op = std::make_unique<Order>(o);
	 std::unique_ptr<Order> op2 = std::make_unique<Order>(o2	);
	 ob.placeOrder(op);
	 ob.placeOrder(op2);
}
