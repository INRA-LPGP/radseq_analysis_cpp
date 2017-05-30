#include "bootstrap.h"

struct Result {
    std::vector<int> combination;
    uint loci;
} best_combination;

// Main bootstrap function: split work into threads and export results
void bootstrap(Infos& infos, const int n_haplotypes, std::bitset<BIT_SIZE>* haplotypes, const int margin, const int n_threads,
               const std::string& output_path, const std::string& log_path) {

    std::ofstream log_file(log_path, std::fstream::app);
    char time[DTTMSZ];

    std::vector<int64_t> n_comb, thread_start;
    thread_start.resize(n_threads+1);

    // Check for 1/3 to 2/3 neomales
    const int min_neomales = std::round(infos.n_males / 3);
    const int max_neomales = std::round(2 * infos.n_males / 3);

    for (int i = min_neomales; i <= max_neomales; ++i){

        n_comb.push_back(get_n_comb(infos.n_males, i));
    }

    uint n_comb_total = std::accumulate(n_comb.begin(), n_comb.end(), 0);

    log_file << "Total number of combinations : " << n_comb_total << std::endl;

    // Results data structures
    std::map<int, int> results;
    std::map<int, int> individuals;

    // Initialize threads and related stuff
    std::vector<std::thread> threads;
    std::mutex results_mutex;

    std::string bitmask;

    int div=0, remainder=0, start=0, end=0;

    for (uint m = 0; m < n_comb.size(); ++m) {

        threads.resize(0);

        log_file << std::endl << "---------- Number of neomales: " << min_neomales + m << " (" << n_comb[m] << " combinations) ----------" << std::endl;

        bitmask = std::string(min_neomales + m, 1); // K leading 1's
        bitmask.resize(infos.n_males, 0); // N-K trailing 0's

        div = std::round(n_comb[m] / n_threads);
        remainder = std::round(n_comb[m] % n_threads);

        thread_start[0] = 0;
        thread_start[n_threads] = n_comb[m];

        for (auto t=1; t<n_threads; ++t) {
            thread_start[t] = thread_start[t-1] + div;
            if (remainder > t-1) {
                ++thread_start[t];
            }
        }

        for (int t = 0; t < n_threads; ++t) {

            start = thread_start[t];
            end = thread_start[t+1] - 1;

            log_file << print_time(time) << "\t" << "Allocating data to thread " << t+1 << " : [" <<start << ", " << end << "]" << std::endl;

            threads.push_back(std::thread(bootstrap_chunk, n_haplotypes, haplotypes, margin, start, end, bitmask, infos.n_males, min_neomales + m,
                                          std::ref(results), std::ref(individuals), std::ref(results_mutex)));

            for (int i=start; i<=end; ++i) std::prev_permutation(bitmask.begin(), bitmask.end());

        }

        for (auto &t : threads) t.join();
    }

    std::ofstream output_file(output_path);
    output_file << "# Loci number results" << std::endl;
    output_file << "Loci_number" << "\t" << "Count" << std::endl;
    for (auto r: results)output_file << r.first << "\t" << r.second << std::endl;
    output_file << std::endl << "# Individual results" << std::endl;
    output_file << "Individual" << "\t" << "Loci_number" << std::endl;
    for (auto i: individuals) output_file << i.first << "\t" << i.second << std::endl;
    output_file << std::endl << "# Best combination" << std::endl;
    for(uint i = 0; i < best_combination.combination.size(); ++i) {
        output_file << best_combination.combination[i];
        if (i != best_combination.combination.size() - 1 ) output_file << ", ";
        else output_file << std::endl;
    }

    log_file << std::endl << print_time(time) << "\t" << "Process ended normally." << std::endl;
}



// Process a chunk of combinations with filter_haplotypes
void bootstrap_chunk(const int n_haplotypes, std::bitset<BIT_SIZE>* haplotypes, const int margin,
                     const int start, const int end, std::string bitmask, const int n_males, const int n_neomales,
                     std::map<int, int>& results, std::map<int, int>& individuals, std::mutex& results_mutex) {

    std::map<int, int> temp_results;
    std::map<int, int> temp_individuals;

    std::bitset<BIT_SIZE> males;
    males.reset();
    std::vector<int> combination;

    uint32_t res = 0;

    for (uint b=0; b<bitmask.size(); ++b) {
        if (bitmask[b]) combination.push_back(b);
    }

    for (int i=start; i<=end; ++i) {
        males.reset();
        for (auto c: combination) males.flip(c);
        res = filter_haplotypes(haplotypes, males, margin, n_haplotypes);
        if (res > best_combination.loci) {
            results_mutex.lock();
            best_combination.combination = combination;
            best_combination.loci = res;
            results_mutex.unlock();
        }
        ++(temp_results[res]);
        for (auto c: combination) temp_individuals[c] += res;
        combination = comb(n_males, n_neomales, bitmask);
    }

    results_mutex.lock();
    for (auto r: temp_results) {
        results[r.first] += r.second;
    }
    for (auto i: temp_individuals) individuals[i.first] += i.second;
    results_mutex.unlock();
}
