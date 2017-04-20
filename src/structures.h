#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

//struct Individual{
//    std::string name;
//    bool male = true;
//};

typedef std::unordered_map<uint16_t, bool> indiv_map; // key: column number, value: name and sex of the individual

typedef std::vector<std::vector<std::string>> hap_map; // vector of vector of haplotypes

typedef std::vector<std::vector<uint16_t>> table;

void visualize(table t);

//void visualize(indiv_map i);

//void visualize(hap_map h);

//void visualize(hap_map h, uint32_t locus);
