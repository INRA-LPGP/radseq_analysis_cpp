#include "haplotypes.h"

int get_numbers(std::string& file_path, int* numbers) {

    std::ifstream haplotype_file(file_path);
    std::string line;
    std::getline(haplotype_file, line);
    haplotype_file.close();

    char sex;
    std::vector<std::string> fields, groups;
    int field_n = 0;

    fields = split(line, "\t"); // Split defined in utils

    for (auto f: fields) {

        groups = split(f, "_");

        if (groups.size() == 3) {

            sex = groups[2][0];

            switch (sex) {
            case 'M':
                ++numbers[0];
                break;
            case 'F':
                ++numbers[1];
                break;
            default:
                break;
            }
        }
        ++field_n;
    }

    return field_n;
}


void get_individual_data(std::string& file_path, bool* indiv_sexes, bool* indiv_col) {

    std::ifstream haplotype_file(file_path);
    std::string line;
    std::getline(haplotype_file, line);
    haplotype_file.close();

    char sex;
    std::vector<std::string> fields, groups;
    int field_n = 0, indiv_n = 0;

    fields = split(line, "\t"); // Split defined in utils

    for (auto f: fields) {

        groups = split(f, "_");

        if (groups.size() == 3) {

            sex = groups[2][0];

            switch (sex) {
            case 'M':
                indiv_col[field_n] = true;
                indiv_sexes[indiv_n] = true;
                ++indiv_n;
                break;
            case 'F':
                indiv_col[field_n] = true;
                indiv_sexes[indiv_n] = false;
                ++indiv_n;
                break;
            default:
                break;
            }
        }
        ++field_n;
    }
}


int number_of_haplotypes(std::string& file_path) {

    std::ifstream haplotype_file(file_path);
    std::string line;
    std::getline(haplotype_file, line);
    int hap_n = 0;

    while (std::getline(haplotype_file, line)) {

        ++hap_n;
    }

    haplotype_file.close();

    return hap_n;
}


void get_haplotypes(std::string& file_path, bool* indiv_col, int** haplotypes, int* haplotype_numbers) {

    std::ifstream haplotype_file(file_path);
    std::string line;
    std::getline(haplotype_file, line);

    std::vector<std::string> fields, temp_haplotypes;
    int field_n = 0, indiv_n = 0, locus_n = 0, hap_n = 0;

    while (std::getline(haplotype_file, line)) {

        temp_haplotypes.resize(0);
        indiv_n = 0, field_n = 0, hap_n = 0;
        fields = split(line, "\t");

        for (auto f: fields) {

            if (indiv_col[field_n]) {

                auto pos = std::find(temp_haplotypes.begin(), temp_haplotypes.end(), f);

                if (pos == temp_haplotypes.end()) {

                    hap_n = temp_haplotypes.size();
                    temp_haplotypes.push_back(f);

                } else {
                    hap_n = std::distance(temp_haplotypes.begin(), pos);
                }

                haplotypes[locus_n][indiv_n] = hap_n;
                ++indiv_n;
            }

            ++field_n;
        }

        haplotype_numbers[locus_n] = temp_haplotypes.size();
        ++locus_n;
    }

    haplotype_file.close();
}


uint32_t filter_haplotypes(int** haplotypes, int* haplotype_numbers, bool* indiv_sexes, const int* margins, const int n_indiv, const int n_haplotypes) {

    // Margins : [males high, males low, females high, females low]
    auto high_bound = std::max_element(haplotype_numbers, haplotype_numbers + n_haplotypes);
    int males[*high_bound];
    int females[*high_bound];

    uint32_t loci_count = 0;

    for (int i = 0; i < n_haplotypes; ++i) {

        for (auto k=0; k<haplotype_numbers[i]; ++k){
            males[k] = 0;
            females[k] = 0;
        }

        for (int j = 0; j < n_indiv; ++j) {

            if (indiv_sexes[j]) ++males[haplotypes[i][j]];
            else ++females[haplotypes[i][j]];
        }

        for (auto k=0; k<haplotype_numbers[i]; ++k) {

            if ((males[k] > margins[0] and females[k] < margins[3]) or (males[k] < margins[1] and females[k] > margins[2])) {
                ++loci_count;
                break;
            }
        }
    }

    return loci_count;
}
