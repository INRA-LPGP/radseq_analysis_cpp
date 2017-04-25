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

        indiv_col[field_n] = false;

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


int number_of_haplotypes(std::string& file_path, bool* indiv_col, bool* indiv_sexes, int margin) {

    std::ifstream haplotype_file(file_path);
    std::string line;
    std::getline(haplotype_file, line);

    std::vector<std::string> fields;
    int field_n = 0, indiv_n = 0, locus_n = 0;

    std::unordered_map<std::string, int> temp_haplotypes;
    std::string top_haplotype;

    std::bitset<BIT_SIZE> haplotype;

    while (std::getline(haplotype_file, line)) {

        temp_haplotypes.clear();
        indiv_n = 0, field_n = 0;
        fields = split(line, "\t");
        top_haplotype = "";

        haplotype.set();

        for (auto f: fields) {

            if (indiv_col[field_n]) {

                if (not indiv_sexes[indiv_n]) ++temp_haplotypes[f];
                ++indiv_n;
            }

            ++field_n;
        }

        field_n = 0, indiv_n = 0;

        for (auto f: fields){

            if (indiv_col[field_n]) {

                if (indiv_sexes[indiv_n]) {

                    // Set haplotype to 1 if present in females and count in females is higher than margin
                    haplotype.set(indiv_n, (temp_haplotypes.count(f) and (temp_haplotypes[f] > margin)));
                }
                ++indiv_n;
            }

            ++field_n;
        }

        if (haplotype.count() < MALE_MIN){
            ++locus_n;
        }
    }

    haplotype_file.close();
    return locus_n;
}


void get_haplotypes(std::string& file_path, bool* indiv_col, bool* indiv_sexes, std::bitset<BIT_SIZE>* haplotypes, int margin) {

    std::ifstream haplotype_file(file_path);
    std::string line;
    std::getline(haplotype_file, line);

    std::vector<std::string> fields;
    int field_n = 0, indiv_n = 0, locus_n = 0;

    std::unordered_map<std::string, int> temp_haplotypes;
    std::string top_haplotype;

    std::bitset<BIT_SIZE> haplotype;

    while (std::getline(haplotype_file, line)) {

        temp_haplotypes.clear();
        indiv_n = 0, field_n = 0;
        fields = split(line, "\t");
        top_haplotype = "";

        haplotype.set();

        for (auto f: fields) {

            if (indiv_col[field_n]) {

                if (not indiv_sexes[indiv_n]) ++temp_haplotypes[f];
                ++indiv_n;
            }

            ++field_n;
        }

        field_n = 0, indiv_n = 0;

        for (auto f: fields){

            if (indiv_col[field_n]) {

                if (indiv_sexes[indiv_n]) {

                    // Set haplotype to 1 if present in females and count in females is higher than margin
                    haplotype.set(indiv_n, (temp_haplotypes.count(f) and (temp_haplotypes[f] > margin)));
                    ++indiv_n;
                }
            }

            ++field_n;
        }

        if (haplotype.count() < MALE_MIN) {
            haplotypes[locus_n] = haplotype;
            ++locus_n;
        }
    }

    haplotype_file.close();
}


uint32_t filter_haplotypes(std::bitset<BIT_SIZE>* haplotypes, std::bitset<BIT_SIZE>& males, const int margin, const int n_haplotypes) {

    uint32_t loci_count = 0;
    int res = 0;

    for (int i = 0; i < n_haplotypes; ++i) {
        res = (haplotypes[i] ^ males).count();
        if (res > margin) ++loci_count;
    }

    return loci_count;
}
