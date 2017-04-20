#include "bootstrap.h"

uint32_t rep(std::vector<int> c, hap_map& haplotypes, indiv_map individuals, table& margins) {

    for (auto i: c) individuals[i] = false;
    return filter_haplotypes(haplotypes, individuals, margins);
}


std::vector<uint32_t> bootstrap(int max_neomales, indiv_map& individuals, int* numbers, hap_map& haplotypes, table& margins) {

    std::vector<uint32_t> results;
    std::vector<std::vector<int>> combinations;

    for (int i = 1; i <= max_neomales; ++i){

        comb(numbers[0], i, combinations);
    }

    for (auto c: combinations){

        results.push_back(rep(c, haplotypes, individuals, margins));
    }

    return results;
}
