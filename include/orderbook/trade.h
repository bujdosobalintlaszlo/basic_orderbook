#pragma once
#include "trade_info.h"
#include <iostream>
#include<vector>
class Trade{
public:
	 Trade(const TradeInfo& bidTrade,const TradeInfo& askTrade) : bidTrade_(bidTrade), askTrade_(askTrade){}

	 TradeInfo getBidTrade() const noexcept;
	 TradeInfo getAskTrade() const noexcept;
	 friend std::ostream& operator<<(std::ostream& os,const Trade& tr);
private:
	 TradeInfo bidTrade_;
	 TradeInfo askTrade_;
};
using Trades = std::vector<Trade>;
