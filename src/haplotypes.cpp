#include "haplotypes.h"

indiv_map get_individual_data(std::string& file_path, int* numbers) {

    std::ifstream haplotype_file(file_path);
    std::string line;
    std::getline(haplotype_file, line);
    haplotype_file.close();

    char sex;
    indiv_map individuals;
    std::vector<std::string> fields, groups;
    uint16_t field_n = 0;

    fields = split(line, "\t"); // Split defined in utils

    for (auto f: fields) {

        groups = split(f, "_");

        if (groups.size() == 3) {

            sex = groups[2][0];

            switch (sex) {
            case 'M':
                individuals[field_n] = true;
                ++numbers[0];
                break;
            case 'F':
                individuals[field_n] = false;
                ++numbers[1];
                break;
            default:
                break;
            }
        }
        ++field_n;
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
        haplotypes.push_back(temp_haplotypes);
        indiv_n = 0, field_n = 0;

        for (auto f: fields) {

            if (individuals.find(field_n) != individuals.end()) {
                haplotypes[locus_n][indiv_n] = f;
                ++indiv_n;
            }

            ++field_n;
        }
        ++locus_n;
    }

    haplotype_file.close();

    std::cout << "Haplotypes found: " << haplotypes.size() << std::endl;

    return haplotypes;
}


uint32_t filter_haplotypes(hap_map& haplotypes, indiv_map& individuals, table& margins) {

    // Margins : [[males high, males low], [females high, females low]]

    uint32_t loci_count = 0;
    std::unordered_map<std::string, std::pair<uint16_t, uint16_t>> genotypes; // TEST WHETHER A NEW MAP OR A CLEAR IS BETTER FOR MAX SIZE N_INDIVIDUALS

    for (uint i = 0; i < haplotypes.size(); ++i) {
        genotypes.clear();

        for (uint j = 0; j < haplotypes[i].size(); ++j) {

            if (individuals[j+2]) ++genotypes[haplotypes[i][j]].first;
            else ++genotypes[haplotypes[i][j]].second;
        }

        for (auto g: genotypes) {

            if ((g.second.first > margins[0][0] and g.second.second < margins[1][1]) or (g.second.first < margins[0][1] and g.second.second > margins[1][0])) {
                ++loci_count;
                break;
            }
        }
    }

    return loci_count;
}
