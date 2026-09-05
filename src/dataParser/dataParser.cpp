#include<iostream>
#include<vector>
#include<fstream>
#include "orderbook/order.h"
#include <stack>
class DataParser{
public:
		  static std::vector<Order> readData(const std::string& filename){
				std::ifstream file(filename);
				std::string line;
				while(std::getline(file,line)){

				}
		  }
};

std::unique_ptr<Order> createOrder(std::string& in){
	 
}

int main(){
	 DataParser d;
	 d.readData("test.json");
	 return 0;
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
