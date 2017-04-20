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


void comb(int N, int K, std::vector<std::vector<int>>& combinations) {

    std::string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's

    std::vector<int> combination(K);
    int c = 0;

    do {
        c = 0;
        for (int i = 0; i < N; ++i) { // [0..N-1] integers

            if (bitmask[i]) {
                combination[c] = i;
                ++c;
            }
        }
        combinations.push_back(combination);
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

}
