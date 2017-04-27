#include "bootstrap.h"

void bootstrap(const int max_neomales, int* numbers, const int n_haplotypes, std::bitset<BIT_SIZE>* haplotypes, const int margin, const int n_threads,
               const std::string& output_path, const std::string& log_path) {

    std::vector<std::vector<int>> combinations;

    for (int i = 0; i <= max_neomales; ++i){

        comb(numbers[0], i, combinations);
    }

    int start = 0, end = 0;
    int chunk_size = std::ceil(combinations.size() / n_threads);

    std::map<int, int> results;
    std::map<int, int> individuals;

    std::ofstream log_file(log_path, std::fstream::app);
    char time[DTTMSZ];

    log_file << "Number of permutations : " << combinations.size() << std::endl;

    std::vector<std::thread> threads;
    std::vector<int> completion;
    for (auto i=0; i<n_threads; ++i) completion.push_back(0);
    threads.resize(0);
    std::mutex results_mutex;

    log_file << std::endl << "---------- Allocating data to threads ----------" << std::endl << std::endl;

    for (int t = 0; t < n_threads; ++t) {

        start = chunk_size * t;
        end = chunk_size * (t + 1) - 1;
        if ((t == n_threads - 1) and (end != int(combinations.size() - 1))) end = combinations.size() - 1;
        log_file << print_time(time) << "\t" << "Allocating data to thread " << t+1 << " : [" <<start << ", " << end << "]" << std::endl;

        threads.push_back(std::thread(bootstrap_chunk, n_haplotypes, haplotypes, margin, std::ref(combinations), start, end,
                                      std::ref(results), std::ref(individuals), std::ref(results_mutex), std::ref(log_file), t+1, std::ref(completion)));

        if (t == 0) results_mutex.lock();
    }

    log_file << std::endl << "---------- Processing data ----------" << std::endl << std::endl;
    results_mutex.unlock();

    for (auto &t : threads) t.join();

    std::ofstream output_file(output_path);
    output_file << "# Loci number results" << std::endl;
    output_file << "Loci_number" << "\t" << "Count" << std::endl;
    for (auto r: results)output_file << r.first << "\t" << r.second << std::endl;
    output_file << std::endl << "# Individual results" << std::endl;
    output_file << "Individual" << "\t" << "Loci_number" << std::endl;
    for (auto i: individuals) output_file << i.first << "\t" << i.second << std::endl;

    log_file << std::endl << print_time(time) << "\t" << "Process ended normally." << std::endl;
}


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
