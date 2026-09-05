#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <list>
#include <unordered_map>
class Order;
struct insertInfo;
using Price = std::uint64_t;
using Quantity = std::uint64_t;
//shpudl replace with string - was uint64_t originaly;
using OrderId = std::string;
using Orders = std::list<std::unique_ptr<Order>>;
using OrderPtr = std::unique_ptr<Order>;
using OrderDate = std::uint64_t;
