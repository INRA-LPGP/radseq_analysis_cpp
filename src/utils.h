#pragma once

#include <vector>
#include <string>

// Splits a std::string into a std::vector of std::strings according to a specified delimiter (default: \t)
std::vector<std::string> split(std::string str, const std::string delimiter);

uint64_t combination(uint64_t n, uint64_t k);
