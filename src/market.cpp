#include "orderbook/market.h"
#include <iostream>
OrderId Market::getId() const {return id_;}
OrderType Market::getOrderType() const {return orderType_;}
Side Market::getSide() const {return side_;}
Quantity Market::getInitialQuantity()const {return initial_quantity_;}
Quantity Market::getRemainingQuantity()const{return remaining_quantity_;}
Quantity Market::filledQuantity() const {return initial_quantity_-remaining_quantity_;}
bool Market::isFilled() const {return initial_quantity_==remaining_quantity_;}
void Market::fill(Quantity quantity){
	 remaining_quantity_-=quantity;
}
double Market::getFufillmentOfOrder() const {
    if (initial_quantity_ == 0) return 0.0;
    return (static_cast<double>(filledQuantity()) / initial_quantity_) * 100.0;
}
void Market::printOrder() const{
	 std::cout << "Order ID: " << getId() 
              << ", Type: " << getOrderType() 
              << ", Side: " << getSide()
              << ", Initial Qty: " << getInitialQuantity() 
              << ", Remaining Qty: " << getRemainingQuantity() 
              << ", Fulfillment: " << getFufillmentOfOrder() << "%\n";
}

