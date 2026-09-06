#include <sstream>
#include <iostream>
#include <memory>
#include "dataParser/dataParser.h"
#include "orderbook/types.h"
#include "orderbook/ordertype.h"
#include "orderbook/order.h" 
#include "orderbook/market.h" 
#include "orderbook/ordertype.h"
std::vector<std::string> DataParser::splitLine(const std::string& line, char delim){
    std::vector<std::string> words;
    std::stringstream s(line);
    std::string word;
    while(std::getline(s, word, delim)){
        words.push_back(word);
    }
    return words;
}

OrderPtr DataParser::createOrder(const std::string& line){
    try{
        auto words = splitLine(line, ',');
		  if(words.size() == 5){
				OrderId id = words.at(0);
				OrderType orderType = static_cast<OrderType>(std::stoi(words.at(1)));
				Side side = static_cast<Side>(std::stoi(words.at(2)));
				Price price = std::stod(words.at(3));
				Quantity quantity = std::stoull(words.at(4));
				return std::make_unique<Order>(id, orderType, side, price, quantity);
		  }
    }catch(const std::exception& e){
        std::cerr << "Failed to parse line: " << e.what() << '\n';
        return nullptr;
    }
	 return nullptr;
}

MarketOrderPtr DataParser::createMarketOrder(const std::string& line){
	 try{
        auto words = splitLine(line, ',');
		  if(words.size() == 4){
				OrderId id = words.at(0);
				OrderType orderType = static_cast<OrderType>(std::stoi(words.at(1)));
				Side side = static_cast<Side>(std::stoi(words.at(2)));
				Quantity quantity = std::stoull(words.at(3));
				return std::make_unique<Market>(id, orderType, side, quantity);
		  }
    }catch(const std::exception& e){
        std::cerr << "Failed to parse line: " << e.what() << '\n';
        return nullptr;
    }
	 return nullptr;
}

/*
{
  "id": "e2a85d9f-07a5-4f94-8d5f-789dc3deb097",
  "method": "order.place",
  "params": {
    "symbol": "BTCUSDT",
    "side": "BUY",
    "type": "LIMIT",
    "price": "0.1",
    "quantity": "10",
    "timeInForce": "GTC",
    "timestamp": 1655716096498,
    "apiKey": "T59MTDLWlpRW16JVeZ2Nju5A5C98WkMm8CSzWC4oqynUlTm1zXOxyauT8LmwXEv9",
    "signature": "5942ad337e6779f2f4c62cd1c26dba71c91514400a24990a3e7f5edec9323f90"
  }
}
*/
