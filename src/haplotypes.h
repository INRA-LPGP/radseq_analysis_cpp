#pragma once
#include "utils.h"
#include <unordered_map>

int get_numbers(std::string& file_path, int* numbers);

void get_individual_data(std::string& file_path, bool* indiv_sexes, bool* indiv_col);

int number_of_haplotypes(std::string& file_path);

void get_haplotypes(std::string& file_path, bool* indiv_col, bool* indiv_sexes, bool** haplotypes, int margin);

uint32_t filter_haplotypes(bool** haplotypes, bool* indiv_sexes, const int margin, const int n_males, const int n_haplotypes);
