#include "orderbook/side.h"
#include<memory>
struct Market{
	 uint64_t id_;
	 Side side_;
	 uint64_t initial_quantity_;
	 uint64_t remaining_quantity_;

	 //setters
	 void fill(uint64_t quantity);
	 uint64_t filledQuantity() const;

	 //only for display
	 double getFufillmentOfOrder() const;
	 
	 bool isFilled() const;

};
