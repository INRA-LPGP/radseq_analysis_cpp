#pragma once
#include <iterator>
#include <math.h>
#include <mutex>
#include <thread>

#include "utils.h"
#include "haplotypes.h"

void bootstrap(const int max_neomales, int* numbers, const int n_haplotypes, std::bitset<BIT_SIZE>* haplotypes, const int margin, const int n_threads);
void bootstrap_chunk(const int n_haplotypes, std::bitset<BIT_SIZE>* haplotypes, const int margin,
                     std::vector<std::vector<int>>& combinations, const int start, const int end,
                     std::map<int, int>& results, std::map<int, int>& individuals, std::mutex& results_mutex);
