#pragma once
#include <iterator>
#include "utils.h"
#include "structures.h"
#include "haplotypes.h"


uint32_t rep(std::vector<int> c, hap_map& haplotypes, bool* indiv_sexes, const int *margins);

std::vector<uint32_t> bootstrap(const int max_neomales, bool* indiv_sexes, int* numbers, const int n_indiv, hap_map &haplotypes, const int* margins);
