#pragma once
#include <iterator>
#include <math.h>
#include <mutex>
#include <thread>
#include <numeric>

#include "haplotypes.h"

void bootstrap(Infos& infos, const int n_haplotypes, std::bitset<BIT_SIZE>* haplotypes, const int margin, const int n_threads,
               const std::string& output_path, const std::string& log_path);
void bootstrap_chunk(const int n_haplotypes, std::bitset<BIT_SIZE>* haplotypes, const int margin,
                     const int start, const int end, std::string bitmask, const int n_males, const int n_neomales,
                     std::map<int, int>& results, std::map<int, int>& individuals, std::mutex& results_mutex);
