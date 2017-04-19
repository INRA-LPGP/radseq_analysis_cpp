#include "utils.h"

// Splits a std::string into a std::vector of std::strings according to a specified delimiter (default: \t)
std::vector<std::string> split(std::string str, const std::string delimiter){

    std::vector<std::string> output;
    size_t pos;

    while ((pos = str.find(delimiter)) != std::string::npos){

        output.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }

    output.push_back(str.substr(0, pos));

    return output;
}


uint64_t
choose(uint64_t n, uint64_t k) {
    if (k > n) {
        return 0;
    }
    uint64_t r = 1;
    for (uint64_t d = 1; d <= k; ++d) {
        r *= n--;
        r /= d;
    }
    return r;
}
