#pragma once
#include <iterator>
#include <math.h>
#include <mutex>
#include <thread>

#include "utils.h"
#include "haplotypes.h"

int bootstrap(const int max_neomales, int* numbers, const int n_haplotypes, std::bitset<64>* haplotypes, const int margin, const int n_threads);
void bootstrap_chunk(int* numbers, const int n_haplotypes, std::bitset<64>* haplotypes, const int margin,
                     std::vector<std::vector<int>>& combinations, int start, int end, std::map<int, int>& results, std::mutex& results_mutex);
