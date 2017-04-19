#include "haplotypes.h"

void get_individual_data(std::string file_path){

    std::ifstream haplotype_file(file_path);

    std::string line;
    char sex;

    Individual indiv;
    indiv_map individuals;

    std::getline(haplotype_file, line);

    std::vector<std::string> fields, groups;

    fields = split(line, "\t"); // Split defined in utils

    uint16_t field_n = 0;

    for (auto f: fields){

        ++field_n;

        groups = split(f, "_");

        if (groups.size() == 3){

            sex = groups[2][0];
            indiv.name = f;

            switch (sex) {
            case 'M':
                indiv.male = true;
            case 'F':
                indiv.male = false;
            }

            individuals[field_n] = indiv;
        }
    }

    for (auto const &i: individuals) std::cout << i.first << " : " << i.second.name << ", " << i.second.male << "\n";


}
