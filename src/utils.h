#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include <time.h>

#define DTTMFMT "%Y-%m-%d %H:%M:%S"
#define DTTMSZ 21

// Output current date and time in format specified with DMTTMFMT and DTTMSZ
char* print_time (char *buff);

// Splits a std::string into a std::vector of std::strings according to a specified delimiter (default: \t)
std::vector<std::string> split(std::string str, const std::string delimiter);

// Generates combinations of K in N
void comb(int N, int K, std::vector<std::vector<int>>& combinations);
