# CMake generated Testfile for 
# Source directory: /home/bujdosobalint/basic_orderbook
# Build directory: /home/bujdosobalint/basic_orderbook/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(OrderBookTests "/home/bujdosobalint/basic_orderbook/build/unit_tests")
set_tests_properties(OrderBookTests PROPERTIES  _BACKTRACE_TRIPLES "/home/bujdosobalint/basic_orderbook/CMakeLists.txt;31;add_test;/home/bujdosobalint/basic_orderbook/CMakeLists.txt;0;")
subdirs("third_party/googletest")
