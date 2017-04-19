#pragma once

#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iterator>
#include "structures.h"
#include "utils.h"

extract_data read_loci_file(std::string file_path);
