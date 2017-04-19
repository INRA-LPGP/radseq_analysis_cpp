#include "haplotypes.h"

indiv_map get_individual_data(std::string& file_path, uint16_t* numbers) {

    std::ifstream haplotype_file(file_path);
    std::string line;
    std::getline(haplotype_file, line);
    haplotype_file.close();

    char sex;
    Individual indiv;
    indiv_map individuals;
    std::vector<std::string> fields, groups;
    uint16_t field_n = 0;

    fields = split(line, "\t"); // Split defined in utils

    for (auto f: fields) {

        ++field_n;
        groups = split(f, "_");

        if (groups.size() == 3) {

            sex = groups[2][0];
            indiv.name = f;

            switch (sex) {
            case 'M':
                indiv.male = true;
                ++numbers[0];
                break;
            case 'F':
                indiv.male = false;
                ++numbers[1];
                break;
            }

            individuals[field_n] = indiv;
        }
    }

    return individuals;
}


hap_map get_haplotypes(std::string& file_path, indiv_map& individuals) {

    std::ifstream haplotype_file(file_path);
    std::string line;
    std::getline(haplotype_file, line);

    hap_map haplotypes;
    std::vector<std::string> fields, temp_haplotypes;
    temp_haplotypes.resize(individuals.size());
    uint16_t field_n = 0, indiv_n = 0;
    uint32_t locus_n = 0;

    while (std::getline(haplotype_file, line)) {

        fields = split(line, "\t");
        locus_n = std::stoul(fields[0]);
        haplotypes.push_back(temp_haplotypes;
        indiv_n = 0, field_n = 0;

        for (auto f: fields) {

            if (individuals.find(field_n) != individuals.end()) {
                haplotypes[locus_n][indiv_n] = f;
                ++indiv_n;
            }

            ++field_n;
        }
    }

    haplotype_file.close();

    return haplotypes;
}


void filter_haplotypes(hap_map& haplotypes, indiv_map& individuals, table& margins) {

    uint32_t loci_count = 0;
    std::unordered_map genotypes; // TEST WHETHER A NEW MAP OR A CLEAR IS BETTER FOR MAX SIZE N_INDIVIDUALS

    for (uint i = 0; i < haplotypes.size(); ++i) {

        for (uint j = 0; j < haplotypes[i].size(); ++j) {

            if (individuals[j+2].male) break; // IMPLEMENT HERE
        }

    }
}
