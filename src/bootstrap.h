#pragma once
#include <iterator>
#include "utils.h"
#include "haplotypes.h"


uint32_t rep(std::vector<int> c, bool** haplotypes, const int margin, const int n_males, const int n_haplotypes);

std::vector<uint32_t> bootstrap(const int max_neomales, int* numbers, const int n_haplotypes, bool** haplotypes, const int margin);
