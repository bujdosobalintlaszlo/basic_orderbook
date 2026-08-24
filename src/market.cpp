#include "orderbook/side.h"
#include<memory>
#include "orderbook/market.h"
//setters
void Market::fill(uint64_t quantity){
	 if(quantity>remaining_quantity_|| quantity <= 0){
		  throw std::invalid_argument("Insuficent quantity!");
	 }
	 remaining_quantity_-=quantity;	  
}
uint64_t Market::filledQuantity() const{
	 return initial_quantity_ - remaining_quantity_;
}

//only for display
double Market::getFufillmentOfOrder() const{
	 if(initial_quantity_ ==0) return 0.0;
	 return static_cast<double>(filledQuantity()) / initial_quantity_ * 100.0;
}

bool Market::isFilled() const {return initial_quantity_ ==remaining_quantity_;} 


