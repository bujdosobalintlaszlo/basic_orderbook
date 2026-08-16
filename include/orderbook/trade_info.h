#pragma once
#include "side.h"
#include <cstdint>
///<summary>Storing object infos related to the trade. (id,price,side,quantity,initial quantity)</summary>
struct TradeInfo{
	 uint64_t orderId_;
	 uint64_t price_;
	 Side side_;
	 uint64_t quantity_;
	 uint64_t initialQuantity_;
 	 TradeInfo(uint64_t orderId,uint64_t price,Side side,uint64_t quantity, uint64_t initialQuantity) : orderId_(orderId),price_(price),side_(side),quantity_(quantity),initialQuantity_(initialQuantity){}
};
