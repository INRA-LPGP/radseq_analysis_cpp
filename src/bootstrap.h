#pragma once
#include <iterator>
#include "utils.h"
#include "haplotypes.h"


uint32_t rep(std::vector<int> c, int**& haplotypes, int* haplotype_numbers, bool* indiv_sexes, const int *margins, const int n_indiv, const int n_haplotypes);

std::vector<uint32_t> bootstrap(const int max_neomales, bool* indiv_sexes, int* numbers, const int n_indiv, const int n_haplotypes, int** &haplotypes, int* haplotype_numbers, const int* margins);
