#include "orderbook/side.h"

std::string to_string(Side side){
	 switch(side){
		  case Side::BUY: return "Buy";
		  case Side::SELL: return "Sell";
	 }
	 return "Unknown side!";
}

std::ostream& operator<<(std::ostream& os,Side side){
	 return os << to_string(side);
}
