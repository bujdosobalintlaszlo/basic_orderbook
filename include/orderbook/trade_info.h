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
};
