#pragma once
#include "utils.h"
#include <unordered_map>
#include <bitset>

#define BIT_SIZE 64

int get_numbers(std::string& file_path, int* numbers);

void get_individual_data(std::string& file_path, bool* indiv_sexes, bool* indiv_col);

int number_of_haplotypes(std::string& file_path, bool* indiv_col, bool* indiv_sexes, int margin);

void get_haplotypes(std::string& file_path, bool* indiv_col, bool* indiv_sexes,  std::bitset<BIT_SIZE>* haplotypes, int margin);

uint32_t filter_haplotypes(std::bitset<BIT_SIZE>* haplotypes, std::bitset<BIT_SIZE>& indiv_sexes, const int margin, const int n_haplotypes);
