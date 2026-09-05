cmake_minimum_required(VERSION 3.5)
project(OrderBook)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external/ftxui)

add_executable(OrderBook 
    src/main.cpp 
    src/order.cpp
    src/orderbook.cpp
    src/ordertype.cpp
    src/side.cpp
    src/trade.cpp
)
target_include_directories(OrderBook PRIVATE include)
target_link_libraries(OrderBook
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)

enable_testing()
add_subdirectory(third_party/googletest)

set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "" FORCE)

add_subdirectory(external/rapidjson)
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
target_link_libraries(unit_tests PRIVATE gtest_main PRIVATE gmock)

add_test(NAME OrderBookTests COMMAND unit_tests)
