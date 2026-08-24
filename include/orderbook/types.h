#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <list>
class Order;

using Price = std::uint64_t;
using Quantity = std::uint64_t;
using OrderId = std::uint64_t;
using Orders = std::list<std::unique_ptr<Order>>;
using OrderPtr = std::unique_ptr<Order>;
using OrderDate = std::uint64_t;
