#include "orderbook/order.h"
#include "orderbook/orderbook.h"
#include "orderbook/side.h"
#include<memory>
#include "orderbook/ordertype.h"
#include "orderbook/trade_info.h"

OrderBook::OrderBook() = default;

const std::map<uint64_t,Orders,std::greater<uint64_t>>& OrderBook::getBids() const noexcept{return bids_;}
const std::map<uint64_t,Orders>& OrderBook::getAsks() const noexcept{return asks_;}

void OrderBook::displayBids() const {
	 std::cout << "___ BIDS DEBUG ___" << '\n';
    for (const auto& [price, order_list] : bids_) {
        for (const auto& order_ptr : order_list) {
				order_ptr->printOrder();
        }
    }
}

void OrderBook::displayAsks() const {
	 std::cout << "___ ASKS DEBUG ___" << '\n';
    for (const auto& [price, order_list] : asks_) {
        for (const auto& order_ptr : order_list) {
				order_ptr->printOrder();
        }
    }
}
template<typename Compare>
Trades OrderBook::matchMarketOrder(OrderPtr &order,std::map<Price,Orders, Compare> &book){
	 Trades trades{};
	 auto it = book.begin();
	 while(it!=book.end() && order->getRemainingQuantity() >0){
		  auto &orders = it->second;
		  auto orders_it = orders.begin();
		  while(orders_it != orders.end() && order->getRemainingQuantity() >0){
				auto &curr_order = *orders_it;
				uint64_t fill_qty{0};
				if(order->getSide() == Side::BUY){
					 fill_qty = std::min(order->getRemainingQuantity(), curr_order->getRemainingQuantity());
				}
				else if(order->getSide() == Side::SELL){
					 fill_qty = std::max(order->getRemainingQuantity(), curr_order->getRemainingQuantity());
				}
				if(fill_qty > 0){
				uint64_t fill_price = curr_order->getPrice();
					 order->fill(fill_qty);
					 curr_order->fill(fill_qty);
					 trades.push_back(createTradeData(order, curr_order));
				}
				if (curr_order->getRemainingQuantity() == 0) {
					 orders_it = orders.erase(orders_it);
				} else {
					 ++orders_it;
				}	
		  }
		  if (orders.empty()) {
				it = book.erase(it);
		  } else {
				++it;
		  }
	 }

	 return trades;
}
///<summary>
///params: the orders pointer, and bids_ or asks_ as a refference depending on BUY or SELL side order.
///return: A vector of trades which went trough. Could be empty, which means it couldn't macth.
///</summary>
template <typename Compare>
Trades OrderBook::matchLimitOrder(OrderPtr &order, std::map<Price,Orders, Compare> &book) {
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
Trade OrderBook::createTradeData(const OrderPtr &bidOrder,const OrderPtr &askOrder){
	  TradeInfo bidInf(bidOrder->getId(),bidOrder->getPrice(),bidOrder->getSide(),bidOrder->getRemainingQuantity(),bidOrder->getInitialQuantity());
	  TradeInfo askInf(askOrder->getId(),askOrder->getPrice(),askOrder->getSide(),askOrder->getRemainingQuantity(),askOrder->getInitialQuantity());
	  Trade trade(bidInf,askInf);
	  return trade;
}

template<typename Comparator>
bool OrderBook::insertIntoBook(OrderPtr &order,std::map<Price,Orders,Comparator> &book){
	 if(!order) return false;
	 const uint64_t price = order->getPrice();
	 const Side side = order->getSide();
	 const uint64_t id = order->getId();
	 auto [it, inserted] = book.try_emplace(price);
	 it->second.push_back(std::move(order));
	 auto order_it = std::prev(it->second.end());
	 OrderBook::InsertInfo i(price,side,order_it);
	 orders_.insert({id,i}); 
	 std::cout << orders_.size() << " SIZE OF ORDERS" << '\n';
	 return true;
}

bool OrderBook::cancelOrder(uint64_t id){
	 auto order_it = orders_.find(id);
	 if(order_it != orders_.end()){
		  if(order_it->second.side_ == Side::BUY){
				return cancel(bids_,order_it);
	 }else{
		  return cancel(asks_,order_it);
		  }
	 }
		  return false;
} 
template<typename BookType>
bool OrderBook::cancel(BookType& book,std::unordered_map<uint64_t,InsertInfo>::iterator order_it){
	 if(order_it != orders_.end()){
		  auto level_it = book.find(order_it->second.price_);
		  if(level_it != book.end()){
				level_it->second.erase(order_it->second.it_);
				if(level_it->second.empty()){
					 book.erase(level_it);
				}
				orders_.erase(order_it);
				return true;
		  }
	 }
	 return false;
}
template<typename Comparator>
Trades OrderBook::FOK(OrderPtr &order,std::map<Price,Orders,Comparator> &book){
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
bool OrderBook::isInBook(const OrderPtr &order) const {
    if (!order) return false;
	 auto it = orders_.find(order->getId());
	 if(it != orders_.end()){
		  return true;
	 }
	 return false;
}
bool OrderBook::placeOrder(OrderPtr order){
	 switch(order->getOrderType()){
		  case OrderType::GoodForDay:
				//implementation postponed
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
					 std::cout << "buyside P only" << '\n';
					 std::cout << "END OF POST ONLY BUY" << '\n';
					 return insertIntoBook(order,bids_);
				}else{
					 std::cout << "sellside P only" << '\n';
					 std::cout << "END OF POST ONLY SELL" << '\n';
					 return insertIntoBook(order,asks_);
				}
				break;
		  
		  case OrderType::IceBerg:
				//implementation postponed
				break;
		  //not in book
		  case OrderType::Market:
				//ps.: INCORRECT IMPLEMENTATION - PRICE SHOULDNT MATTER IT EXECUTES AT WHATEVER PRICE
				if(order->getSide() == Side::BUY){
					 matchMarketOrder(order,asks_);
				}
				else{
					 matchMarketOrder(order,bids_);
				}
				std::cout << "A MARKET Order has been filled with Order ID: " << order->getId() << ", with " << order->getRemainingQuantity() << " remaining unfilled." << '\n';
				break;
		  case OrderType::FillAndKill:
				if(order->getSide() == Side::BUY){
					 matchMarketOrder(order,asks_);
				}
				else{
					 matchMarketOrder(order,bids_);
				}
				break;

		  case OrderType::FillOrKill:
				if(order->getSide() == Side::BUY){
					 FOK(order,asks_);
				}
				else{
					 FOK(order,bids_);
				}
				break;
		  case OrderType::Limit:
				if(order->getSide() == Side::BUY){
					 matchLimitOrder(order,bids_);
				}else{
					 matchLimitOrder(order,asks_);
				}
				break;
	 }
	 return true;
}

/*
 * Notes for ordertypes:
 * ___NOT LISTED IN THE BOOK___
 * - Market: Executed immediately no matter the price. Remaining amount is cancelled (or rejected if no liquidity). Won't get listed in the book.
 * - FillAndKill (Immediate-Or-Cancel -> IOC): Fills as much as it can at the given limit price or better. Remaining amount is cancelled immediately. Won't sit in the book.
 * - FillOrKill (FOK): If the order can be executed FULLY immediately at the given price or better, execute it. Otherwise cancel/reject the entire order. Won't sit in the book.
 * 
 * ___LISTED IN THE BOOK___
 * - GoodTillCancel (GTC): Stays in the book until fully filled or explicitly cancelled by the user.
 * - GoodForDay (GFD): Stays in the book until the end of the trading day (then auto-cancelled by the exchange).
 * 
 *
 * - PostOnly: Must ONLY act as a Maker (sit in the book). If it would execute immediately against an existing order (acting as a Taker), it gets cancelled instead of placed.
 * - IceBerg: A large order broken into smaller visible segments (e.g., 10,000 total size, but only displays 100 at a time in the book). Once a visible slice is filled, the next slice is pushed to the book.
 */
