#include "utils.h"

// Output current date and time in format specified in utils.h
char* print_time (char *buff) {

    time_t t = time (0);
    strftime (buff, DTTMSZ, DTTMFMT, localtime (&t));
    return buff;
}



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


// Generates combinations of K in N
std::vector<int> comb(int N, int K, std::string& bitmask) {

    std::vector<int> combination(K);
    std::prev_permutation(bitmask.begin(), bitmask.end());
    int c = 0;

    for (int i = 0; i < N; ++i) { // [0..N-1] integers
        if (bitmask[i]) {
            combination[c] = i;
            ++c;
        }
    }

    return combination;
}


int64_t get_n_comb(int N, int K) {

    return int64_t(std::tgamma(N+1) / (std::tgamma(K+1) * std::tgamma(N-K+1)));
}
