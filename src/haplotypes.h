#pragma once
#include "utils.h"
#include <unordered_map>
#include <bitset>

#define BIT_SIZE 64
#define MALE_MIN 61

const int get_numbers(const std::string& file_path, int* numbers);

void get_individual_data(const std::string& file_path, bool* indiv_sexes, bool* indiv_col);

const int number_of_haplotypes(const std::string& file_path, bool* indiv_col, bool* indiv_sexes, const int margin);

void get_haplotypes(const std::string& file_path, bool* indiv_col, bool* indiv_sexes,  std::bitset<BIT_SIZE>* haplotypes, const int margin);

const uint32_t filter_haplotypes(std::bitset<BIT_SIZE>* haplotypes, std::bitset<BIT_SIZE>& indiv_sexes, const int margin, const int n_haplotypes);
