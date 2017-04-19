#pragma once

#include <map>
#include <string>

struct Individual{
    std::string name;
    bool male = true;
};

// Default string - string map
typedef std::map<std::string, std::string> ss_dict;

typedef std::map<uint16_t, Individual> indiv_map;


