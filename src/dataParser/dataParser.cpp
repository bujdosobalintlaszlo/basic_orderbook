#include<iostream>
#include<vector>
#include<fstream>
#include "orderbook/order.h"
#include <stack>
#include <string>
#include<stdio.h>
#include <sstream>
#include<cstdint>
//#include "orderbook/types.h"
class DataParser{
private:
		  static std::vector<std::unique_ptr<Order>> parseCSV(const std::string& filename){
				try{
					 std::fstream f;
					 f.open(filename);
					 std::string temp,line,word;
					 std::vector<std::string> words;
					 std::vector<OrderPtr> orders;
					 while(f>>temp){
						  words.clear();
						  getline(f,line);
						  std::stringstream s(line);
						  while(getline(s,word,',')){
								words.push_back(word);
						  }
						  std::unique_ptr<Order> o = createOrder(words);
						  if(o == nullptr){
								return {};
						  }
						  orders.push_back(std::move(o));
						  o->printOrder();
					 }
					 return orders;
				}catch(...){
					 return {};
				}
		  }
		  static std::vector<std::unique_ptr<Order>> parseJSON(const std::string& filename){
				
		  }

		  static std::unique_ptr<Order> createOrder(const std::vector<std::string> &data) {
				try{
					 OrderId id = data[0];
					 OrderType orderType = static_cast<OrderType>(std::stoi(data[1]));
					 Side side = static_cast<Side>(std::stoi(data[2])) ;
					 Price price;
					 std::istringstream iss_price(data[3]);
					 iss_price >> price;
					 Quantity quantity;
					 std::istringstream iss_qnt(data[3]);
					 iss_qnt >> quantity;
					 return std::make_unique<Order>(Order(id,orderType,side,price,quantity));
				}catch(...){
					 return nullptr;
				}
		  }

public:
		  static std::vector<std::unique_ptr<Order>> readData(const std::string& filename){
				//invalid filename -- NOTE might throw
				if(filename.size() <= 0) return {};
				std::stack<char> fileType;
				int i = filename.size();
				std::string fileTypeStr;
				while(filename[i] != '.'){
					 fileType.push(filename[i]);
				}
				while(!fileType.empty()){
					 fileTypeStr.push_back(fileType.top());
					 fileType.pop();
				}
				if(fileTypeStr == "csv"){
					 return parseCSV(filename);
				}else if(fileTypeStr == "json"){
					 return parseJSON(filename);
				}

				//invalid file type
				return {};
		  }

};

std::unique_ptr<Order> createOrder(std::string& in){
	 
}

int main(){
	 DataParser d;
	 d.readData("cstest.csv");
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
