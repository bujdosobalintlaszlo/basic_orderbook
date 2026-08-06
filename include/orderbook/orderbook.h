#pragma once
#include<map>
#include<memory>
#include<list>
#include "order.h"
#include "trade.h"
#include <unordered_map>
using Orders = std::list<std::unique_ptr<Order>>;
using OrderPtr = std::unique_ptr<Order>;
using Price = uint64_t;
class OrderBook{
private:
	 std::map<int64_t,Orders,std::greater<int64_t>> bids_;
	 std::map<int64_t,Orders> asks_;
	 std::unordered_map<uint64_t,OrderPtr> orders_;
	 Trades trades_;
public:
	 const std::map<int64_t,Orders,std::greater<int64_t>>& getBids() const noexcept;
	 const std::map<int64_t,Orders>& getAsks() const noexcept;
	 OrderBook();

	 void placeOrder(std::unique_ptr<Order> order);
	 bool cancelOrder(uint64_t orderId);
	 void matchOrder(std::unique_ptr<Order> order);
	 void displayBids() const;
	 void displayAsks() const;
	 void displayTrades() const; 
	 void processOrder(std::unique_ptr<Order> order);
	 bool MatchOrderOnBuy(std::unique_ptr<Order> &order);
	 bool MatchOrderOnSell(std::unique_ptr<Order> &order);
	 template<typename BookType>
	 bool FOK(BookType &book,std::unique_ptr<Order> &order);
	 bool canMatch(Side side,Price price) const;
};

