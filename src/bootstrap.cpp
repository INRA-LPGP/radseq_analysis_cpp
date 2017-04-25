#include "bootstrap.h"

int bootstrap(const int max_neomales, int* numbers, const int n_haplotypes, std::bitset<64>* haplotypes, const int margin, const int n_threads) {

    std::vector<std::vector<int>> combinations;

    for (int i = 1; i <= max_neomales; ++i){

        comb(numbers[0], i, combinations);
    }

    int start = 0, end = 0;
    int chunk_size = std::ceil(combinations.size() / 4);

    std::map<int, int> results;

    std::cout << "Number of permutations : " << combinations.size() + 1 << std::endl;

    std::vector<std::thread> threads;
    threads.resize(0);
    std::mutex results_mutex;

    for (int t = 0; t < n_threads; ++t) {

        start = chunk_size * t;
        end = chunk_size * (t + 1) - 1;
        if ((t == n_threads - 1) and (end != int(combinations.size()) - 1)) end = combinations.size();
        std::cout << "Chunk " << t << " : " <<start << ", " << end << std::endl;

        threads.push_back(std::thread(bootstrap_chunk, std::ref(numbers), n_haplotypes, haplotypes, margin, std::ref(combinations), start, end, std::ref(results), std::ref(results_mutex)));
    }

    for (auto &t : threads) t.join();

//    for (auto i=0; i<n_haplotypes; ++i) delete[] haplotypes[i];
//    delete[] haplotypes;

    std::cout << "Results :" << std::endl;
    for (auto r: results) std::cout << r.first << " : " << r.second << std::endl;

    return 0;
}


void bootstrap_chunk(int* numbers, const int n_haplotypes, std::bitset<64>* haplotypes, const int margin,
                     std::vector<std::vector<int>>& combinations, int start, int end, std::map<int, int>& results, std::mutex& results_mutex) {

    std::map<int, int> temp_results;

    std::bitset<64> males;

    for (int i=start; i<end; ++i){

        males.set();
        for (auto c: combinations[i]) males.flip(c);

        ++(temp_results[filter_haplotypes(haplotypes, males, margin, numbers[0], n_haplotypes)]);
    }

    results_mutex.lock();
    for (auto r: temp_results) results[r.first] += r.second;
    results_mutex.unlock();
}
