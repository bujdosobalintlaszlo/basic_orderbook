#pragma once
#include<map>
#include<memory>
#include<list>
#include<unordered_map>
#include "order.h"
#include "trade.h"
#include <unordered_map>
using Orders = std::list<std::unique_ptr<Order>>;
using OrderPtr = std::unique_ptr<Order>;
using Price = uint64_t;
class OrderBook{
private:
	 struct InsertInfo{
		  uint64_t price_;
		  Side side_;
		  Orders::iterator it_;
		  InsertInfo(uint64_t price,Side side,Orders::iterator it) : price_(price),side_(side),it_(it){};

	 };
	 std::map<uint64_t,Orders,std::greater<uint64_t>> bids_;
	 std::map<uint64_t,Orders> asks_;
	 std::unordered_map<uint64_t,InsertInfo> orders_;

public:
	 const std::map<uint64_t,Orders,std::greater<uint64_t>>& getBids() const noexcept;
	 const std::map<uint64_t,Orders>& getAsks() const noexcept;
	 OrderBook();
	 Trades placeOrder(OrderPtr order);
	 bool isInBook(const OrderPtr &order) const;
	 void displayBids() const;
	 void displayAsks() const;
	 template <typename Compare>
	 Trades matchMarketOrder(OrderPtr &order, std::map<Price,Orders, Compare> &book);
	 template<typename Comparator>
	 bool insertIntoBook(OrderPtr &order,std::map<Price,Orders,Comparator> &book);
	 Trade createTradeData(const OrderPtr &bidOrder,const OrderPtr &askOrder);
	 bool cancelOrder(uint64_t id);
	 template<typename Comparator>
	 Trades FOK(OrderPtr &order,std::map<Price,Orders,Comparator> &book);
	 template<typename BookType>
	 bool cancel(BookType& book,std::unordered_map<uint64_t,InsertInfo>::iterator order_it);
	 template<typename Compare>
	 Trades matchLimitOrder(OrderPtr &order, std::map<Price,Orders, Compare> &book); 
	 template<typename BookType>
	 Trades matchMarketOrder(OrderPtr &order,BookType &book);

};

