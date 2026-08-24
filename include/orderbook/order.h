#pragma once
#include "side.h"
#include "ordertype.h"
#include "optional"
#include<iostream>
#include<memory>
class Order{
private:
	 uint64_t id_;
	 OrderType orderType_;
	 Side side_;
	 uint64_t price_;
	 uint64_t initial_quantity_;
	 uint64_t remaining_quantity_;
	 double convertToDecimal(uint64_t price) const;
public:
	 Order(uint64_t id,OrderType orderType,Side side,uint64_t price,uint64_t quantity);
	 
	 //getters
	 uint64_t getId() const;
	 OrderType getOrderType() const;
	 Side getSide()const;
	 uint64_t getPrice()const;
	 uint64_t getInitialQuantity()const;
	 uint64_t getRemainingQuantity()const;
	 bool hasPrice() const;
	 //setters
	 void fill(uint64_t quantity);
	 uint64_t filledQuantity() const;

	 //only for display
	 double getFufillmentOfOrder() const;
	 void printOrder() const;
	 bool isFilled() const; 

};
