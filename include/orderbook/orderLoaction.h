#pragma once
#include <list>
#include "order.h"
#include "side.h"
#include <map>
class OrderLocation{
	 uint64_t id_;
	 uint64_t price_;
	 Side side_;
	 std::list<Order>::iterator list_it_;
	 OrderLocation(uint64_t id,uint64_t price,Side side,std::list<Order>::iterator it) : id_(id),price_(price),side_(side),list_it_(it) {} 
};
