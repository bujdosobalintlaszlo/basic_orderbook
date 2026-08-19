cmake_minimum_required(VERSION 3.28)

project(OrderBook)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#program
add_executable(OrderBook 
    src/main.cpp 
    src/order.cpp
    src/orderbook.cpp
	 src/ordertype.cpp
	 src/side.cpp
	 src/trade.cpp
)

target_include_directories(OrderBook PRIVATE include)

enable_testing()
add_subdirectory(third_party/googletest)

#test
add_executable(unit_tests
    test/orderbook_test.cpp
	 test/order_test.cpp
    src/order.cpp
    src/orderbook.cpp
	 src/ordertype.cpp
	 src/side.cpp
	 src/trade.cpp
)

target_include_directories(unit_tests PRIVATE include)
target_link_libraries(unit_tests PRIVATE gtest_main)

add_test(NAME OrderBookTests COMMAND unit_tests)
