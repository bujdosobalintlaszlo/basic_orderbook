#include "order.h"
#include "orderbook.h"
#include "side.h"
#include<string> 
#include<memory>
#include "ordertype.h"
#include "trade_info.h"
OrderBook::OrderBook() = default;
void OrderBook::placeOrder(std::unique_ptr<Order> order) {
    if (!order) return;

    switch (order->getSide()) {
        case Side::BUY: {
            uint64_t price = order->getPrice();
            auto [it, inserted] = bids_.try_emplace(price);
            it->second.push_back(std::move(order));
            break;
        }

        case Side::SELL: {
            uint64_t price = order->getPrice();
            auto [it, inserted] = asks_.try_emplace(price);
            it->second.push_back(std::move(order));
            break;
        }

        default:
            throw std::invalid_argument("Faulty order with invalid Side! Order ID: " + std::to_string(order->getId()));
    }
}
const std::map<int64_t,Orders,std::greater<int64_t>>& OrderBook::getBids() const noexcept{return bids_;}
const std::map<int64_t,Orders>& OrderBook::getAsks() const noexcept{return asks_;}

void OrderBook::displayTrades() const {
	 std::cout << "___ Trades ___" << '\n';
	 for(auto it = trades_.begin();it!=trades_.end();++it){
	 }
}

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

void OrderBook::matchOrder(std::unique_ptr<Order> order){
	 switch(order->getOrderType()){
		  //into book
		  case OrderType::GoodForDay:
				break;		
		 
		  //can be filled immidatelly, inserted if couldnt fill
		  case OrderType::GoodTillCancel:
				if(order->getSide() == Side::BUY){
					 if(MatchOrderOnBuy(order)){
						  if(order->getRemainingQuantity() > 0){
									 uint64_t price = order->getPrice();
									 auto [it, inserted] = bids_.try_emplace(price);
									 it->second.push_back(std::move(order));
						  }
					 }else{
						  uint64_t price = order->getPrice();
						  auto [it, inserted] = bids_.try_emplace(price);
						  it->second.push_back(std::move(order));

					 }
			  }
				else{
					 if(MatchOrderOnSell(order)){
						 if(order->getRemainingQuantity()> 0){
								uint64_t price = order->getPrice();
								auto [it, inserted] = asks_.try_emplace(price);
								it->second.push_back(std::move(order));
						 } 
					 }else{
						  uint64_t price = order->getPrice();
						  auto [it, inserted] = asks_.try_emplace(price);
						  it->second.push_back(std::move(order));
					 }
				}
				break;
		  //done 
		  case OrderType::PostOnly:
				if(order->getSide() == Side::BUY){
					 uint64_t price = order->getPrice();
					 auto [it, inserted] = bids_.try_emplace(price);
					 it->second.push_back(std::move(order));
				}else{
					 uint64_t price = order->getPrice();
					 auto [it, inserted] = asks_.try_emplace(price);
					 it->second.push_back(std::move(order));
				}
				break;
		  
		  case OrderType::IceBerg:
				break;
		  //not in book
		  case OrderType::Market:
				if (order->getSide() == Side::BUY) {
					 MatchOrderOnBuy(order);
            } else {
					 MatchOrderOnSell(order);
            }
				std::cout << "A MARKET Order has been filled with Order ID: " << order->getId() << ", with " << order->getRemainingQuantity() << " remaining unfilled." << '\n';
				break;
		  case OrderType::FillAndKill:
				break;
		  case OrderType::FillOrKill:
				bool passed = false;
				if(order->getSide() == Side::BUY){
					passed = FOK(bids_,order); 
				}else{
					passed = FOK(asks_,order); 
				}
				if(passed){
					 std::cout << "SIKERES FOK";
				}else{
					 std::cout << "SIKERTELEN FOK";
				}
				break;
	 }
}
bool OrderBook::MatchOrderOnBuy(std::unique_ptr<Order> &order){
	 auto it = asks_.begin();
	 while (it != asks_.end() && order->getRemainingQuantity() > 0) {
		  auto& ask_list = it->second;
		  auto ask_it = ask_list.begin();

		  while (ask_it != ask_list.end() && order->getRemainingQuantity() > 0) {
				auto& ask_order = *ask_it;
				// Calculate fill quantity based on REMAINING amounts
				uint64_t fillable_quant = std::min(order->getRemainingQuantity(), ask_order->getRemainingQuantity());
				uint64_t fill_prc = ask_order->getPrice();

				// Deduct filled quantity
				ask_order->setQuantity(fillable_quant);
				order->setQuantity(fillable_quant);

				// Record Trade
				TradeInfo bid_inf(order->getId(), fill_prc, Side::BUY, order->getRemainingQuantity(), order->getInitialQuantity());
				TradeInfo ask_inf(ask_order->getId(), fill_prc, Side::SELL, ask_order->getRemainingQuantity(), ask_order->getInitialQuantity());
				trades_.emplace_back(bid_inf, ask_inf);

				// If the passive ask order is fully filled, erase it and get the next iterator
				if (ask_order->getRemainingQuantity() == 0) {
					 ask_it = ask_list.erase(ask_it);
				} else {
					 ++ask_it;
				}
		  }
		  // If the price level is completely empty, safely erase the price level
		  if (ask_list.empty()) {
				it = asks_.erase(it);
		  } else {
				++it;
		  }
	 }
	 return order->getRemainingQuantity() != order->getInitialQuantity();
}
bool OrderBook::canMatch(Side side,Price price) const {
	 if(side == Side::BUY){
		  if(asks_.empty()) return false;
		  const auto& [bestAsk, _] = *asks_.begin();
		  return price >= bestAsk;

	 }else{
		  if(bids_.empty()) return false;
		  const auto& [bestBid,_] = *bids_.begin();
		  return price <= bestBid;
	 }
}
bool OrderBook::cancelOrder(uint64_t id) {
    auto orderIt = orders_.find(id);
    if (orderIt == orders_.end()) {
        return false;
    }
	  
    
    return true;
}
bool OrderBook::MatchOrderOnSell(std::unique_ptr<Order> &order){
	 auto it =bids_.begin();
                while (it != bids_.end() && order->getRemainingQuantity() > 0) {
                    auto& bid_list = it->second;
                    auto bid_it = bid_list.begin();

                    while (bid_it != bid_list.end() && order->getRemainingQuantity() > 0) {
                        auto& bid_order = *bid_it;
                        // Calculate fill quantity based on REMAINING amounts
						  uint64_t fillable_quant = std::max(order->getRemainingQuantity(), bid_order->getRemainingQuantity());
						  uint64_t fill_prc = bid_order->getPrice();

						  // Deduct filled quantity
						  bid_order->setQuantity(fillable_quant);
						  order->setQuantity(fillable_quant);
						  // Record Trade
						  TradeInfo ask_inf(order->getId(), fill_prc, Side::SELL, order->getRemainingQuantity(), order->getInitialQuantity());
						  TradeInfo bid_inf(bid_order->getId(), fill_prc, Side::BUY, bid_order->getRemainingQuantity(), bid_order->getInitialQuantity());
						  trades_.emplace_back(bid_inf,ask_inf);

						  // If the passive ask order is fully filled, erase it and get the next iterator
						  if (bid_order->getRemainingQuantity() == 0) {
								bid_it = bid_list.erase(bid_it);
						  } else {
								++bid_it;
						  }
					 }
					 // If the price level is completely empty, safely erase the price level
					 if (bid_list.empty()) {
						  it = bids_.erase(it);
					 } else {
						  ++it;
					 }
				}
		  return order->getRemainingQuantity() != order->getInitialQuantity();
}
template<typename BookType>
bool OrderBook::FOK(BookType &book,std::unique_ptr<Order> &order){
	 auto it = book.begin();
	 while(it != book.end()){
		  auto& orders = it->second;
		  auto order_it = orders.begin();
		  while(order_it != orders.end()){
				auto& book_order = *order_it;
				std::cout << "Aktualis order:" << '\n';
				book_order->printOrder();
				if(book_order->getInitialQuantity() == order->getInitialQuantity()){
					 if(order->getSide() == Side::BUY && order->getPrice() >= book_order->getPrice()){
						  uint64_t fill_prc = std::min(order->getPrice(),book_order->getPrice());
						  TradeInfo bid_inf(order->getId(), fill_prc, Side::BUY, order->getRemainingQuantity(), order->getInitialQuantity());
						  TradeInfo ask_inf(book_order->getId(), fill_prc, Side::SELL, book_order->getRemainingQuantity(), book_order->getInitialQuantity());
						  trades_.emplace_back(bid_inf, ask_inf);
						  return true;
					 }else{
						  uint64_t fill_prc = std::max(order->getPrice(),book_order->getPrice());
						  TradeInfo bid_inf(order->getId(), fill_prc, Side::BUY, order->getRemainingQuantity(), order->getInitialQuantity());
						  TradeInfo ask_inf(book_order->getId(), fill_prc, Side::SELL, book_order->getRemainingQuantity(), book_order->getInitialQuantity());
						  trades_.emplace_back(bid_inf, ask_inf);
						  return true;
					 }
				}
				++order_it;
		  }
		  ++it;
	 }
	 return false;
}
/*
bool OrderBook::FOK(std::map<int64_t,Orders,std::greater<int64_t>> & book,std::unique_ptr<Order> &order){
	 auto it = book.begin();
	 while(it != book.end() && order->getRemainingQuantity() > 0){
		  auto& order_list = it->second;
		  auto order_it = order_list.begin();
		  while(order_it != order_list.end()){
				auto &alias = *order_it;
				if(order->getInitialQuantity() == alias->getInitialQuantity()){
					 uint64_t fill_prc = alias->getPrice();
					 alias->setQuantity(order->getInitialQuantity());
					 order->setQuantity(order->getInitialQuantity());
					 if(order->getSide() == Side::BUY){
						  TradeInfo bid(order->getId(),fill_prc,Side::BUY,0,order->getInitialQuantity());
						  TradeInfo ask(alias->getId(),fill_prc,Side::SELL,alias->getRemainingQuantity(),alias->getInitialQuantity());
						  trades_.emplace_back(bid,ask);
						  return true;
					 }else{
						  TradeInfo bid(order->getId(),fill_prc,Side::BUY,0,order->getInitialQuantity());
						  TradeInfo ask(alias->getId(),fill_prc,Side::SELL,alias->getRemainingQuantity(),alias->getInitialQuantity());
						  trades_.emplace_back(ask,bid);
						  return true;
					 }
				}	
				++order_it;
		  }
		  ++it;
	 }
	 return false;
}
bool OrderBook::FOK(std::map<int64_t,Orders> & book,std::unique_ptr<Order> &order){
	 auto it = book.begin();
	 while(it != book.end() && order->getRemainingQuantity() > 0){
		  auto& order_list = it->second;
		  auto order_it = order_list.begin();
		  while(order_it != order_list.end()){
				auto &alias = *order_it;
				if(order->getInitialQuantity() == alias->getInitialQuantity()){
					 uint64_t fill_prc = alias->getPrice();
					 alias->setQuantity(order->getInitialQuantity());
					 order->setQuantity(order->getInitialQuantity());
					 if(order->getSide() == Side::BUY){
						  TradeInfo bid(order->getId(),fill_prc,Side::BUY,0,order->getInitialQuantity());
						  TradeInfo ask(alias->getId(),fill_prc,Side::SELL,alias->getRemainingQuantity(),alias->getInitialQuantity());
						  trades_.emplace_back(bid,ask);
						  return true;
					 }else{
						  TradeInfo bid(order->getId(),fill_prc,Side::BUY,0,order->getInitialQuantity());
						  TradeInfo ask(alias->getId(),fill_prc,Side::SELL,alias->getRemainingQuantity(),alias->getInitialQuantity());
						  trades_.emplace_back(ask,bid);
						  return true;
					 }
				}	
				++order_it;
		  }
		  ++it;
	 }
	 return false;
}
*/
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
