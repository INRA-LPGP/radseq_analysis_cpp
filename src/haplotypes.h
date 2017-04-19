#pragma once
#include "utils.h"
#include "structures.h"

indiv_map get_individual_data(std::string& file_path, uint16_t* numbers);

hap_map get_haplotypes(std::string& file_path, indiv_map& individuals);

void filter_haplotypes(hap_map& haplotypes, indiv_map& individuals, table& margins);
