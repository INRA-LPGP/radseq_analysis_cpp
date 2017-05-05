#pragma once
#include "utils.h"
#include <unordered_map>
#include <bitset>

#define BIT_SIZE 64 // Length of bitset is multiple of 32. Assumption: number of males < 64. If number of males is higher, can be increased to 96 or 128.
#define MALE_MAX (BIT_SIZE - 3) // Threshold to filter haplotypes: if less than 3 original males diverge from females, haplotype is discarded.

const int get_numbers(const std::string& file_path, int* numbers);

void get_individual_data(const std::string& file_path, bool* indiv_sexes, bool* indiv_col);

const std::pair<int, int> number_of_haplotypes(const std::string& file_path, bool* indiv_col, bool* indiv_sexes, const int margin);

void get_haplotypes(const std::string& file_path, bool* indiv_col, bool* indiv_sexes,  std::bitset<BIT_SIZE>* haplotypes, const int margin);

const uint32_t filter_haplotypes(std::bitset<BIT_SIZE>* haplotypes, std::bitset<BIT_SIZE>& indiv_sexes, const int margin, const int n_haplotypes);
