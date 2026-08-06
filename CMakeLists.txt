cmake_minimum_required(VERSION 3.14)

project(OrderBook CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(OrderBook 
    src/main.cpp 
    src/order.cpp
	 src/orderbook.cpp
)

target_include_directories(OrderBook PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
