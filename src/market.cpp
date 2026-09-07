#include "orderbook/market.h"
#include <iostream>
OrderId Market::getId() const {return id_;}
OrderType Market::getOrderType() const {return orderType_;}
Side Market::getSide() const {return side_;}
Quantity Market::getInitialQuantity()const {return initial_quantity_;}
Quantity Market::getRemainingQuantity()const{return remaining_quantity_;}
Quantity Market::filledQuantity() const {return initial_quantity_-remaining_quantity_;}
Symbol Market::getSymbol() const {return symbol_;}
bool Market::isFilled() const {return initial_quantity_==remaining_quantity_;}
void Market::fill(Quantity quantity){
	 remaining_quantity_-=quantity;
}
double Market::getFufillmentOfOrder() const {
    if (initial_quantity_ == 0) return 0.0;
    return (static_cast<double>(filledQuantity()) / initial_quantity_) * 100.0;
}
bool Market::setQuantity(Quantity qnt){
	if(qnt < 0 || qnt == remaining_quantity_ || (remaining_quantity_ == initial_quantity_ && remaining_quantity_ == qnt)){
		  throw std::invalid_argument("An invalid quantity was given at ID: "+id_);
	}
	if(qnt < initial_quantity_){
		  remaining_quantity_=qnt;
	}else{
		  initial_quantity_=qnt;
		  remaining_quantity_=qnt;
	}
	 return true;	
}
void Market::printOrder() const{
	 std::cout << "Order ID: " << getId() 
              << ", Type: " << getOrderType() 
              << ", Side: " << getSide()
              << ", Initial Qty: " << getInitialQuantity() 
              << ", Remaining Qty: " << getRemainingQuantity() 
              << ", Fulfillment: " << getFufillmentOfOrder() << "%\n";
}

