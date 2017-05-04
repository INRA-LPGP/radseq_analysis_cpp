#include "bootstrap.h"

// Main bootstrap function: split work into threads and export results
void bootstrap(const int max_neomales, int* numbers, const int n_haplotypes, std::bitset<BIT_SIZE>* haplotypes, const int margin, const int n_threads,
               const std::string& output_path, const std::string& log_path) {

    std::ofstream log_file(log_path, std::fstream::app);
    char time[DTTMSZ];

    std::vector<int> n_comb, thread_start;
    thread_start.resize(n_threads+1);

    for (int i = 1; i <= max_neomales; ++i){

        n_comb.push_back(get_n_comb(numbers[0], i));
    }

    log_file << "Number of permutations : " << std::accumulate(n_comb.begin(), n_comb.end(), 0) << std::endl;

    // Results data structures
    std::map<int, int> results;
    std::map<int, int> individuals;

    // Initialize threads and related stuff
    std::vector<std::thread> threads;
    std::vector<int> completion;
    for (auto i=0; i<n_threads; ++i) completion.push_back(0);
    std::mutex results_mutex;

    std::string bitmask;

    int div=0, remainder=0, start=0, end=0;

    for (int m = 0; m < max_neomales; ++m) {

        bitmask = std::string(max_neomales, 1); // K leading 1's
        bitmask.resize(numbers[0], 0); // N-K trailing 0's

        div = std::round(n_comb[m] / n_threads);
        remainder = std::round(n_comb[m] % n_threads);

        thread_start[0] = 0;
        thread_start[n_threads] = n_comb[m]+1;

        for (auto t=1; t<n_threads; ++t) {
            thread_start[t] = thread_start[t-1] + div;
            if (remainder > t-1) {
                ++thread_start[t];
            }
        }

        log_file << std::endl << "---------- Allocating data to threads ----------" << std::endl << std::endl;

        for (int t = 0; t < n_threads; ++t) {

            start = thread_start[t];
            end = thread_start[t+1] - 1;
            std::cout << std::endl << start << " , " << end << std::endl;

            log_file << print_time(time) << "\t" << "Allocating data to thread " << t+1 << " : [" <<start << ", " << end << "]" << std::endl;

            for(auto b: bitmask) std::cout << bool(b);
            std::cout<<std::endl<<std::endl;

            std::bitset<BIT_SIZE> males;
            std::vector<int> combination;
            std::string bitmask_c(bitmask);

            for (int i=start; i<end; ++i){
                males.set();
                for (auto c: combination) males.flip(c);
                combination = comb(numbers[0], m + 1, bitmask_c);
                for (auto c: combination) std::cout << c << ", ";
                std::cout << std::endl;
            }

//            threads.push_back(std::thread(bootstrap_chunk, n_haplotypes, haplotypes, margin, start, end, std::ref(bitmask), numbers[0], m,
//                                          std::ref(results), std::ref(individuals), std::ref(results_mutex), std::ref(log_file), t+1, std::ref(completion)));

            for (int i=start; i<end; ++i) std::prev_permutation(bitmask.begin(), bitmask.end());

            if (t == 0) results_mutex.lock();
        }

        log_file << std::endl << "---------- Processing data ----------" << std::endl << std::endl;
        results_mutex.unlock();

//        for (auto &t : threads) t.join();
    }

    std::ofstream output_file(output_path);
    output_file << "# Loci number results" << std::endl;
    output_file << "Loci_number" << "\t" << "Count" << std::endl;
    for (auto r: results)output_file << r.first << "\t" << r.second << std::endl;
    output_file << std::endl << "# Individual results" << std::endl;
    output_file << "Individual" << "\t" << "Loci_number" << std::endl;
    for (auto i: individuals) output_file << i.first << "\t" << i.second << std::endl;

    log_file << std::endl << print_time(time) << "\t" << "Process ended normally." << std::endl;
}

// Process a chunk of combinations with filter_haplotypes
void bootstrap_chunk(const int n_haplotypes, std::bitset<BIT_SIZE>* haplotypes, const int margin,
                     const int start, const int end, std::string& bitmask_ref, const int n_males, const int n_neomales,
                     std::map<int, int>& results, std::map<int, int>& individuals, std::mutex& results_mutex,
                     std::ofstream& log_file, int thread_number, std::vector<int>& completion) {

//    std::cout << "Neomales: " << n_neomales << ", Thread: " << thread_number << std::endl;
    char time[DTTMSZ];
    std::string bitmask(bitmask_ref);

    std::map<int, int> temp_results;
    std::map<int, int> temp_individuals;

    std::bitset<BIT_SIZE> males;
    males.set();
    std::vector<int> combination;

    int chunk_size = end - start;
    int chunk_step = int(std::ceil(chunk_size / 10));
    if (chunk_step == 0) chunk_step = 1;
    int step_count = 0;
    int res = 0;
    bool step_completed = true;

    for (int i=start; i<end; ++i){

        if (i % chunk_step == 0 or i == end-1) {
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

        res = filter_haplotypes(haplotypes, males, margin, n_haplotypes);
        ++(temp_results[res]);
        for (auto c: combination) temp_individuals[c] += res;

        males.set();
        if (i+1 < end) {
            combination = comb(n_males, n_neomales, bitmask);
            for (auto c: combination) males.flip(c);
        }
    }

    results_mutex.lock();
    for (auto r: temp_results) results[r.first] += r.second;
    for (auto i: temp_individuals) individuals[i.first] += i.second;
    results_mutex.unlock();
}
