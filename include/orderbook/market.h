#pragma once
#include "side.h"
#include "types.h"
#include "ordertype.h"
class Market{
private:
	 OrderId id_;
	 OrderType orderType_;
	 Side side_;
	 Quantity initial_quantity_;
	 Quantity remaining_quantity_;
	 Date date_;
public:
	 Market(OrderId id,OrderType orderType,Side side,Quantity init_quant,Date date) : id_(id), orderType_(orderType),side_(side), initial_quantity_(init_quant), remaining_quantity_(init_quant),date_(date) {}
	 //getters
	 OrderId getId() const;
	 OrderType getOrderType() const;
	 Side getSide() const;
	 Quantity getInitialQuantity()const;
	 Quantity getRemainingQuantity()const;
	 Quantity filledQuantity() const;
	 bool isFilled() const;
	 //setters
	 bool setQuantity(Quantity qnt);
	 void fill(Quantity quantity);
	 
	 //display only/debug
	 void printOrder() const;
	 double getFufillmentOfOrder() const;
};
