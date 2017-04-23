#pragma once
#include <iterator>
#include <math.h>
#include "utils.h"
#include "haplotypes.h"

int bootstrap(const int max_neomales, int* numbers, const int n_haplotypes, bool** haplotypes, const int margin);
void bootstrap_chunk(int* numbers, const int n_haplotypes, bool** haplotypes, const int margin,
                     std::vector<std::vector<int>>& combinations, int start, int end, std::map<int, int>& results);
