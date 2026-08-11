#include "orderbook/ordertype.h"

std::string to_string(OrderType op) {
    switch (op) {
        case OrderType::GoodTillCancel: 
            return "Good till cancel";
        case OrderType::FillAndKill:
            return "Fill and kill";
        case OrderType::FillOrKill:
            return "Fill or kill";
        case OrderType::GoodForDay:
            return "Good for day";
        case OrderType::Market:
            return "Market";
        case OrderType::Limit:
            return "Limit";
        case OrderType::PostOnly:
            return "PostOnly";
        case OrderType::IceBerg:
            return "IceBerg";
    }
    return "Unknown Order type!";
}

std::ostream& operator<<(std::ostream& os, OrderType op) {
    return os << to_string(op);
}
