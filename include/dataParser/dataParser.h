#pragma once
#include <string>
#include <vector>
#include "orderbook/types.h"
class DataParser{
public:
    static OrderPtr createOrder(const std::string& line);
    static std::vector<std::string> splitLine(const std::string& line, char delim);
	 static MarketOrderPtr createMarketOrder(const std::string& line);
};
