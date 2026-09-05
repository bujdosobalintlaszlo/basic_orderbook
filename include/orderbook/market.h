#include "orderbook/side.h"
#include "types.h"
struct Market{
	 OrderId id_;
	 Side side_;
	 Quantity initial_quantity_;
	 Quantity remaining_quantity_;

	 //setters
	 void fill(Quantity quantity){
		 remaining_quantity_-=quantity; 
	 }
	 Quantity filledQuantity() const;

	 //only for display
	 double getFufillmentOfOrder() const;
	 
	 bool isFilled() const;
};
