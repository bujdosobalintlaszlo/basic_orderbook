#pragma once
#include<string>
#include <iosfwd>
enum class Side {BUY,SELL};

std::string to_string(Side side);
std::ostream& operator<<(std::ostream& os,Side side);

