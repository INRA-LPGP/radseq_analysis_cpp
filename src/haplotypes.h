#pragma once
#include "utils.h"
#include <unordered_map>
#include <bitset>

#define BIT_SIZE 64 // Length of bitset is multiple of 32. Assumption: number of males < 64. If number of males is higher, can be increased to 96 or 128.
#define MALE_MAX (BIT_SIZE - 10) // Threshold to filter haplotypes: if less than 3 original males diverge from females, haplotype is discarded.

struct Infos {
    std::vector<bool> columns;
    std::vector<bool> sexes;
    uint n_males = 0;
    uint n_females = 0;
};

Infos get_infos(const std::string& file_path);

const std::pair<int, int> number_of_haplotypes(const std::string& file_path, const Infos& infos, const int margin);

void get_haplotypes(const std::string& file_path, const Infos& infos,  std::bitset<BIT_SIZE>* haplotypes, const int margin);

const uint32_t filter_haplotypes(const std::bitset<BIT_SIZE>* haplotypes, const std::bitset<BIT_SIZE>& indiv_sexes, const int margin, const int n_haplotypes);
