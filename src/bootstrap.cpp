#include "bootstrap.h"

uint32_t rep(std::vector<int> c, int**& haplotypes, int* haplotype_numbers, bool* indiv_sexes, const int* margins, const int n_indiv, const int n_haplotypes) {

    for (auto i: c) indiv_sexes[i] = false;
    return filter_haplotypes(haplotypes, haplotype_numbers, indiv_sexes, margins, n_indiv, n_haplotypes);
}


std::vector<uint32_t> bootstrap(const int max_neomales, bool* indiv_sexes, int* numbers, const int n_indiv, const int n_haplotypes, int** &haplotypes, int* haplotype_numbers, const int *margins) {

    std::vector<uint32_t> results;
    std::vector<std::vector<int>> combinations;

    for (int i = 1; i <= max_neomales; ++i){

        comb(numbers[0], i, combinations);
    }

    std::cout << "Number of permutations : " << combinations.size() + 1 << std::endl;

    for (auto c: combinations){

        bool new_indiv_sexes[n_indiv];
        for (auto i=0; i<n_indiv; ++i) new_indiv_sexes[i] = indiv_sexes[i];
        results.push_back(rep(c, haplotypes, haplotype_numbers, new_indiv_sexes, margins, n_indiv, n_haplotypes));
    }

    return results;
}
