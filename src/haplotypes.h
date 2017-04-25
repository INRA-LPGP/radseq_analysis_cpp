#pragma once
#include "utils.h"
#include <unordered_map>
#include <bitset>

int get_numbers(std::string& file_path, int* numbers);

void get_individual_data(std::string& file_path, bool* indiv_sexes, bool* indiv_col);

int number_of_haplotypes(std::string& file_path);

void get_haplotypes(std::string& file_path, bool* indiv_col, bool* indiv_sexes,  std::bitset<64>* haplotypes, int margin);

uint32_t filter_haplotypes(std::bitset<64>* haplotypes, std::bitset<64>& indiv_sexes, const int margin, const int n_males, const int n_haplotypes);
