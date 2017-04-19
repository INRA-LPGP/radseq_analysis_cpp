#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <unordered_map>

typedef std::map<std::string, std::vector<std::string>> locus;

typedef std::map<std::string, locus> individual;

typedef std::unordered_map<std::string, std::string> correspondance;

typedef std::unordered_map<std::string, correspondance> extract_data;

void display(individual& i);

void display(extract_data& d);

void display(correspondance& c);
