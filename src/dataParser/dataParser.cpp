#include<iostream>
#include<vector>
#include<fstream>
#include "orderbook/order.h"
class DataParser{
public:
		  static std::vector<Order> readData(const std::string& filename){
				std::vector<Order> orders;
				try{
					 std::ifstream stream{filename};
					 std::string line;
					 while(getline(stream,line)){
						  std::cout << line << '\n';	  
					 }
				}catch(std::exception e){
					 throw e;
				}
				return orders;

		  }
};

int main(){
	 DataParser d;
	 d.readData("test.csv");
	 return 0;
}
