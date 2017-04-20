#pragma once
#include "utils.h"
#include "structures.h"

int get_numbers(std::string& file_path, int* numbers);

void get_individual_data(std::string& file_path, bool* indiv_sexes, bool* indiv_col);

hap_map get_haplotypes(std::string& file_path, bool* indiv_col, const int n_indiv);

uint32_t filter_haplotypes(hap_map& haplotypes, bool* indiv_sexes, const int* margins);
