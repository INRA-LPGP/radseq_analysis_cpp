#pragma once
#include "utils.h"
#include "structures.h"
#include "haplotypes.h"

uint32_t rep(std::vector<int> c, hap_map& haplotypes, indiv_map individuals, table& margins);

std::vector<uint32_t> bootstrap(int max_neomales, indiv_map& individuals, int* numbers, hap_map &haplotypes, table &margins);
