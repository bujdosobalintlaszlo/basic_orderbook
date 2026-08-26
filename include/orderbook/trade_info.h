#pragma once
#include "side.h"
#include "types.h"
///<summary>Storing object infos related to the trade. (id,price,side,quantity,initial quantity)</summary>
struct TradeInfo{
	 OrderId orderId_;
	 Price price_;
	 Side side_;
	 Quantity quantity_;
	 Quantity initialQuantity_;
 	 TradeInfo(OrderId orderId,Price price,Side side,Quantity quantity, Quantity initialQuantity) : orderId_(orderId),price_(price),side_(side),quantity_(quantity),initialQuantity_(initialQuantity){}
	 bool operator==(const TradeInfo& other) const {
		  return orderId_ == other.orderId_ && price_ == other.price_ &&
					side_ == other.side_ && quantity_ == other.quantity_ &&
					initialQuantity_ == other.initialQuantity_;
	 }

	 bool operator!=(const TradeInfo& other) const {
		  return !(*this == other);
	 }
};
