#pragma once

#include <fstream>
#include <algorithm>
#include "gzstream.h"
#include "structures.h"
#include "utils.h"

void process_file(std::string& file_path, extract_data& loci_to_extract);

correspondance::const_iterator process_line(std::string& line, extract_data& loci_to_extract, individual& loci);
