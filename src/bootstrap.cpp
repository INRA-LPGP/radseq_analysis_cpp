#include "bootstrap.h"

uint32_t rep(std::vector<int> c, hap_map& haplotypes, bool* indiv_sexes, const int* margins) {

    for (auto i: c) indiv_sexes[i] = false;
    return filter_haplotypes(haplotypes, indiv_sexes, margins);
}


std::vector<uint32_t> bootstrap(const int max_neomales, bool* indiv_sexes, int* numbers, const int n_indiv, hap_map &haplotypes, const int *margins) {

    std::vector<uint32_t> results;
    std::vector<std::vector<int>> combinations;

    for (int i = 1; i <= max_neomales; ++i){

        comb(numbers[0], i, combinations);
    }

    for (auto c: combinations){

        bool new_indiv_sexes[n_indiv];
        for (auto i=0; i<n_indiv; ++i) new_indiv_sexes[i] = indiv_sexes[i];
        results.push_back(rep(c, haplotypes, new_indiv_sexes, margins));
    }

    return results;
}
