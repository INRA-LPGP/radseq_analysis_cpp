#include "bootstrap.h"

int bootstrap(const int max_neomales, int* numbers, const int n_haplotypes, bool** haplotypes, const int margin) {

    std::vector<std::vector<int>> combinations;

    for (int i = 1; i <= max_neomales; ++i){

        comb(numbers[0], i, combinations);
    }

    int start = 0, end = 0;
    int n_threads = 4;
    int chunk_size = std::ceil(combinations.size() / 4);

    std::map<int, int> results;

    std::cout << "Number of permutations : " << combinations.size() + 1 << std::endl;

    for (int t = 0; t < n_threads; ++t) {

        start = chunk_size * t;
        end = chunk_size * (t + 1) - 1;
        if ((t == n_threads - 1) and (end != int(combinations.size()) - 1)) end = combinations.size() - 1;
        std::cout << "Chunk " << t << " : " <<start << ", " << end << std::endl;
        bootstrap_chunk(numbers, n_haplotypes, haplotypes, margin, combinations, start, end, results);
    }

//    bool males[numbers[0]];

//    for (auto c: combinations){

//        for (auto i=0; i<numbers[0]; ++i) males[i] = true;
//        for (auto i: c) males[i] = false;

//        ++(results[filter_haplotypes(haplotypes, males, margin, numbers[0], n_haplotypes)]);
//    }

    for (auto i=0; i<n_haplotypes; ++i) delete[] haplotypes[i];
    delete[] haplotypes;

    std::cout << "Results :" << std::endl;
    for (auto r: results) std::cout << r.first << " : " << r.second << std::endl;

    return 0;
}


void bootstrap_chunk(int* numbers, const int n_haplotypes, bool** haplotypes, const int margin,
                     std::vector<std::vector<int>>& combinations, int start, int end, std::map<int, int>& results) {

    std::map<int, int> temp_results;

    bool males[numbers[0]];

    for (int i=start; i<end; ++i){

        for (auto i=0; i<numbers[0]; ++i) males[i] = true;
        for (auto c: combinations[i]) males[c] = false;

        ++(temp_results[filter_haplotypes(haplotypes, males, margin, numbers[0], n_haplotypes)]);
    }
    for (auto r: temp_results) results[r.first] += r.second;
}
