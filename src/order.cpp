#include "orderbook/order.h"
#include <iostream>
#include <stdexcept>
#include <string>

Order::Order(uint64_t id, OrderType orderType, Side side, double price, uint64_t quantity) 
    : id_(id), orderType_(orderType), side_(side), price_(price), 
      initial_quantity_(quantity), remaining_quantity_(quantity) {
      
    if (quantity == 0) {
        throw std::invalid_argument("Order quantity cannot be zero! Order ID: " + std::to_string(id));
    }
    if (price <= 0.0) {
        throw std::invalid_argument("Order price must be positive! Order ID: " 
            + std::to_string(id) + " with PRICE: " + std::to_string(price));
    }
}

// Getters
uint64_t Order::getId() const { return id_; }
OrderType Order::getOrderType() const { return orderType_; }
Side Order::getSide() const { return side_; }
double Order::getPrice() const { return price_; }
uint64_t Order::getInitialQuantity() const { return initial_quantity_; }
uint64_t Order::getRemainingQuantity() const { return remaining_quantity_; }

// Modifiers & Calculations
void Order::fill(uint64_t quantity) {
    if (quantity > remaining_quantity_) {
        throw std::invalid_argument("Tried to fill more than remaining quantity!");
    }
    remaining_quantity_ -= quantity;
}

uint64_t Order::filledQuantity() const {
    return initial_quantity_ - remaining_quantity_;
}

double Order::getFufillmentOfOrder() const {
    if (initial_quantity_ == 0) return 0.0;
    return (static_cast<double>(filledQuantity()) / initial_quantity_) * 100.0;
}

bool Order::isFilled() const { 
    return remaining_quantity_ == 0; 
}

void Order::printOrder() const {
    std::cout << "Order ID: " << getId() 
              << ", Type: " << static_cast<int>(getOrderType()) 
              << ", Side: " << (getSide() == Side::BUY ? "Buy" : "Sell")
              << ", Price: " << getPrice() 
              << ", Initial Qty: " << getInitialQuantity() 
              << ", Remaining Qty: " << getRemainingQuantity() 
              << ", Fulfillment: " << getFufillmentOfOrder() << "%\n";
}
