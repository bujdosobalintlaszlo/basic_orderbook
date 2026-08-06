#pragma once
#include "side.h"
#include "ordertype.h"
#include<iostream>
#include<memory>
class Order{
public:
	 Order(uint64_t id,OrderType orderType,Side side,double price,uint64_t quantity);
	 
	 //getters
	 uint64_t getId() const;
	 OrderType getOrderType()const;
	 Side getSide()const;
	 double getPrice()const;
	 uint64_t getInitialQuantity()const;
	 uint64_t getRemainingQuantity()const;
	 
	 //setters
	 void fill(uint64_t quantity);
	 uint64_t filledQuantity() const;

	 //only for display
	 double getFufillmentOfOrder() const;
	 void printOrder() const;
	 bool isFilled() const; 
private:
	 uint64_t id_;
	 OrderType orderType_;
	 Side side_;
	 int64_t price_;
	 uint64_t initial_quantity_;
	 uint64_t remaining_quantity_;
};
