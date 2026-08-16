#include "orderbook/trade.h"
#include "orderbook/trade_info.h"
TradeInfo Trade::getBidTrade() const noexcept {return bidTrade_;}
TradeInfo Trade::getAskTrade() const noexcept {return askTrade_;}

std::ostream& operator<<(std::ostream& os, const Trade &trade) {
    return os << "| Bid Trade: Id: " << trade.getBidTrade().orderId_ 
              << ", Price: " << trade.getBidTrade().price_ 
              << ", Quantity: " << trade.getBidTrade().quantity_ << "|\n"
              << "| Ask Trade: Id: " << trade.getAskTrade().orderId_ 
              << ", Price: " << trade.getAskTrade().price_ 
              << ", Quantity: " << trade.getAskTrade().quantity_ << "|\n";
}

