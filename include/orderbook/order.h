#pragma once
#include "side.h"
#include "ordertype.h"
#include "types.h"
class Order{
private:
	 OrderId id_;
	 OrderType orderType_;
	 Side side_;
	 Price price_;
	 Quantity initial_quantity_;
	 Quantity remaining_quantity_;
	 double convertToDecimal(uint64_t price) const;
public:
	 Order(OrderId id,OrderType orderType,Side side,Price price,Quantity quantity);
	 Order(OrderId id, Side side, Quantity quantity);
	 //getters
	 uint64_t getId() const;
	 OrderType getOrderType() const;
	 Side getSide()const;
	 Price getPrice()const;
	 Quantity getInitialQuantity()const;
	 Quantity getRemainingQuantity()const;
	 bool hasPrice() const;
	 //setters
	 void fill(uint64_t quantity);
	 uint64_t filledQuantity() const;

	 //only for display
	 double getFufillmentOfOrder() const;
	 void printOrder() const;
	 bool isFilled() const; 

};
