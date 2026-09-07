#pragma once
#include "side.h"
#include "ordertype.h"
#include "types.h"
/*
!!!IMPORTANT!!! timeInforce: order can be limit and have a time enforcer like GTC
have to be careful at parse
Example imp;
modId,uuid,orderType(0-8 for now 7 cause gfd not implemented yet),side(0-1),price(uint64_t),quantity(uint64_t),date(uint64_t),symbol(string)
symbols:
	 b - buy -> upper mentioned
	 c - cancel - needs id only -> c,uuid
	 m - modify - needs id, (ONLY price and quantity) -> modifyId(p || q) and the amount
 * */
class Order{
private:
	 OrderId id_;
	 OrderType orderType_;
	 Side side_;
	 Price price_;
	 Quantity initial_quantity_;
	 Quantity remaining_quantity_;
	 Date date_;
	 Symbol symbol_;
	 double convertToDecimal(Price price) const;
public:
	 Order(OrderId id,OrderType orderType,Side side,Price price,Quantity quantity, Date date, Symbol symbol);
	 //getters
	 OrderId getId() const;
	 OrderType getOrderType() const;
	 Side getSide()const;
	 Price getPrice()const;
	 Quantity getInitialQuantity()const;
	 Quantity getRemainingQuantity()const;
	 Date getDate() const;
	 Symbol getSymbol() const;
	 //setters
	 void fill(Quantity quantity);
	 Quantity filledQuantity() const;
	 bool setQuantity(Quantity qnt);
	 bool setPrice(Price price);

	 //only for display
	 double getFufillmentOfOrder() const;
	 void printOrder() const;
	 bool isFilled() const; 

};
