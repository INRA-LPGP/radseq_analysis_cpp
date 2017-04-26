#include "bootstrap.h"

void bootstrap(const int max_neomales, int* numbers, const int n_haplotypes, std::bitset<BIT_SIZE>* haplotypes, const int margin, const int n_threads) {

    std::vector<std::vector<int>> combinations;

    for (int i = 0; i <= max_neomales; ++i){

        comb(numbers[0], i, combinations);
    }

    int start = 0, end = 0;
    int chunk_size = std::ceil(combinations.size() / n_threads);

    std::map<int, int> results;
    std::map<int, int> individuals;

    std::cout << "Number of permutations : " << combinations.size() << std::endl;

    std::vector<std::thread> threads;
    threads.resize(0);
    std::mutex results_mutex;

    std::cout << std::endl << "---------- Creating threads ----------" << std::endl << std::endl;

    for (int t = 0; t < n_threads; ++t) {

        start = chunk_size * t;
        end = chunk_size * (t + 1) - 1;
        if ((t == n_threads - 1) and (end != int(combinations.size() - 1))) end = combinations.size() - 1;
        std::cout << "Thread " << t << " : " <<start << ", " << end << std::endl;

        threads.push_back(std::thread(bootstrap_chunk, n_haplotypes, haplotypes, margin, std::ref(combinations), start, end,
                                      std::ref(results), std::ref(individuals), std::ref(results_mutex)));
    }

    for (auto &t : threads) t.join();

    std::cout << std::endl << "Results :" << std::endl;
    for (auto r: results) std::cout << r.first << " : " << r.second << std::endl;
    std::cout << std::endl;
    for (auto i: individuals) std::cout << i.first << " : " << i.second << std::endl;
}


void bootstrap_chunk(const int n_haplotypes, std::bitset<BIT_SIZE>* haplotypes, const int margin,
                     std::vector<std::vector<int>>& combinations, const int start, const int end,
                     std::map<int, int>& results, std::map<int, int>& individuals, std::mutex& results_mutex) {

    std::map<int, int> temp_results;
    std::map<int, int> temp_individuals;

    std::bitset<BIT_SIZE> males;

    int chunk_size = end - start;
    int chunk_10p = std::round(chunk_size / 10);
    if (chunk_10p == 0) chunk_10p = 1;
    int p_count = 0;
    int res = 0;

    for (int i=start; i<=end; ++i){

        if (i % chunk_10p == 0){
            std::cerr << "Processed " << p_count * 10 << " %" << std::endl;
            p_count++;
        }

        males.set();
        for (auto c: combinations[i]) males.flip(c);
        res = filter_haplotypes(haplotypes, males, margin, n_haplotypes);
        ++(temp_results[res]);
        for (auto c: combinations[i]) temp_individuals[c] += res;
    }

    results_mutex.lock();
    for (auto r: temp_results) results[r.first] += r.second;
    for (auto i: temp_individuals) individuals[i.first] += i.second;
    results_mutex.unlock();
}
