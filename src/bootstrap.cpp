#include "bootstrap.h"

uint32_t rep(std::vector<int> c, bool** haplotypes, const int margin, const int n_males, const int n_haplotypes) {

    bool males[n_males];
    for (auto i=0; i<n_males; ++i) males[i] = true;
    for (auto i: c) males[i] = false;
    return filter_haplotypes(haplotypes, males, margin, n_males, n_haplotypes);
}


std::vector<uint32_t> bootstrap(const int max_neomales, int* numbers, const int n_haplotypes, bool** haplotypes, const int margin) {

    std::vector<uint32_t> results;
    std::vector<std::vector<int>> combinations;

    for (int i = 1; i <= max_neomales; ++i){

        comb(numbers[0], i, combinations);
    }

    std::cout << "Number of permutations : " << combinations.size() + 1 << std::endl;

    for (auto c: combinations){

        results.push_back(rep(c, haplotypes, margin, numbers[0], n_haplotypes));
    }

    return results;
}
