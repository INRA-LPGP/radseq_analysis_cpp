#include "bootstrap.h"

// Main bootstrap function: split work into threads and export results
void bootstrap(const int max_neomales, int* numbers, const int n_haplotypes, std::bitset<BIT_SIZE>* haplotypes, const int margin, const int n_threads,
               const std::string& output_path, const std::string& log_path) {

    std::vector<int> comb_numbers, thread_start;
    thread_start.resize(n_threads);

    for (int i = 0; i <= max_neomales; ++i){
        comb_numbers.push_back(number_of_combinations(numbers[0], i));
    }

    std::string bitmask(max_neomales, 1); // K leading 1's
    bitmask.resize(numbers[0], 0); // N-K trailing 0's

    std::vector<std::thread> threads;

    for (int m=0; m<max_neomales; ++m) {

        chunk_div = std::floor(comb_numbers[m] / n_threads);
        chunk_mod = comb_numbers % n_threads;
        thread_start[0] = 0;
        for (auto t=1; t < n_threads; ++t) {
        }

        for (int t = 0; t < n_threads; ++t) {

            start = chunk_size * t;
            end = chunk_size * (t + 1) - 1;
            if ((t == n_threads - 1) and (end != int(combinations.size() - 1))) end = combinations.size() - 1;

            threads.push_back(std::thread(bootstrap_chunk, n_haplotypes, haplotypes, margin, std::ref(combinations), start, end,
                                          std::ref(results), std::ref(individuals), std::ref(results_mutex), std::ref(log_file), t+1, std::ref(completion)));

        }




        for (auto &t : threads) t.join();
    }
}


// Process a chunk of combinations with filter_haplotypes
void bootstrap_chunk(const int n_haplotypes, std::bitset<BIT_SIZE>* haplotypes, const int margin,
                     std::vector<std::vector<int>>& combinations, const int start, const int end,
                     std::map<int, int>& results, std::map<int, int>& individuals, std::mutex& results_mutex,
                     std::ofstream& log_file, int thread_number, std::vector<int>& completion) {

    std::map<int, int> temp_results;
    std::map<int, int> temp_individuals;

    std::bitset<BIT_SIZE> males;

    int chunk_size = end - start;
    int chunk_step = int(std::ceil(chunk_size / 10));
    if (chunk_step == 0) chunk_step = 1;
    int step_count = 0;
    int res = 0;
    bool step_completed = true;
    char time[DTTMSZ];

    for (int i=start; i<=end; ++i){

        if (i % chunk_step == 0 or i == end) {
            step_completed = true;
            completion[thread_number-1] = step_count;
            for (auto t: completion) if (t < step_count) step_completed = false;
            if (step_completed) {
                results_mutex.lock();
                log_file << print_time(time) << "\t" << "Completed " << step_count * 10 << " %" << std::endl;
                results_mutex.unlock();
            }
            ++step_count;
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
