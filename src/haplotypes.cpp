#include "haplotypes.h"


Infos get_infos(const std::string& file_path) {

    struct Infos infos;

    std::ifstream haplotype_file(file_path);
    std::string line;
    std::getline(haplotype_file, line);

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
                ++infos.n_males;
                infos.columns.push_back(true);
                infos.sexes.push_back(true);
                break;
            case 'F':
                ++infos.n_females;
                infos.columns.push_back(true);
                infos.sexes.push_back(false);
                break;
            default:
                break;
            }

        } else {

            infos.columns.push_back(false);
        }

        ++field_n;
    }

    haplotype_file.close();
    return infos;
}




const std::pair<int, int> number_of_haplotypes(const std::string& file_path, const Infos& infos, const int margin) {

    std::ifstream haplotype_file(file_path);
    std::string line;
    std::getline(haplotype_file, line);

    std::vector<std::string> fields, alleles;
    bool all_alleles;
    int field_n = 0, indiv_n = 0, locus_n = 0, total_n = 0;

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

            if (infos.columns[field_n]) {

                if (not infos.sexes[indiv_n]) {

                    if (f.find('/') == std::string::npos) {

                        ++temp_haplotypes[f];

                    } else {

                        alleles = split(f, "/");
                        for (auto a: alleles) {
                            ++temp_haplotypes[a];
                        }
                    }
                }

                ++indiv_n;
            }

            ++field_n;
        }

        field_n = 0, indiv_n = 0;

        for (auto f: fields){

            if (infos.columns[field_n]) {

                if (infos.sexes[indiv_n]) {

                    // Set haplotype to 1 if present in females and count in females is higher than margin
                    if (f.find('/') == std::string::npos) {

                        haplotype.set(indiv_n, (temp_haplotypes.count(f) and (temp_haplotypes[f] > margin)));

                    } else {

                        // Case with several genotypes (or alleles) : check each genotype / allele
                        alleles = split(f, "/");
                        all_alleles = true;

                        for (auto a: alleles) {
                            if (not (temp_haplotypes.count(a) and (temp_haplotypes[a] > margin))) {
                                all_alleles = false;
                            }
                        }

                        haplotype.set(indiv_n, all_alleles);
                    }
                }
                ++indiv_n;
            }
            ++field_n;
        }

        if (haplotype.count() < MALE_MAX) { // Haplotype is discarded if too few males diverge from females (no sex specific signal at all)
            ++locus_n;
        }
        ++total_n;
    }

    haplotype_file.close();
    return std::pair<int, int>(locus_n, total_n);
}




void get_haplotypes(const std::string& file_path, const Infos& infos, std::bitset<BIT_SIZE>* haplotypes, const int margin) {

    std::ifstream haplotype_file(file_path);
    std::string line;
    std::getline(haplotype_file, line);

    std::vector<std::string> fields, alleles;
    bool all_alleles;
    int field_n = 0, indiv_n = 0, locus_n = 0, male_n = 0;

    std::unordered_map<std::string, int> temp_haplotypes;

    std::bitset<BIT_SIZE> haplotype;

    std::ofstream test;
    test.open("haplotypes.tsv");

    while (std::getline(haplotype_file, line)) {

        temp_haplotypes.clear();
        indiv_n = 0, field_n = 0;
        fields = split(line, "\t");
        haplotype.set();

        for (auto f: fields) {

            if (infos.columns[field_n]) {

                if (not infos.sexes[indiv_n]) {

                    if (f.find('/') == std::string::npos) {

                        ++temp_haplotypes[f];

                    } else {

                        alleles = split(f, "/");
                        for (auto a: alleles) {
                            ++temp_haplotypes[a];
                        }
                    }
                }

                ++indiv_n;
            }

            ++field_n;
        }

        field_n = 0, indiv_n = 0, male_n = 0;

        for (auto f: fields){

            if (infos.columns[field_n]) {

                if (infos.sexes[indiv_n]) {

                    // Set haplotype to 1 if present in females and count in females is higher than margin
                    if (f.find('/') == std::string::npos) {

//                        std::cout << f << " : " << indiv_n << " - " << infos.n_males << " - " << indiv_n - infos.n_males << std::endl;

                        haplotype.set(male_n, (temp_haplotypes.count(f) and (temp_haplotypes[f] > margin)));

                    } else {

                        // Case with several genotypes (or alleles) : check each genotype / allele
                        alleles = split(f, "/");
                        all_alleles = true;

                        for (auto a: alleles) {
                            if (not (temp_haplotypes.count(a) and (temp_haplotypes[a] > margin))) {
                                all_alleles = false;
                            }
                        }

//                        std::cout << f << " : " << indiv_n - infos.n_males << std::endl;

                        haplotype.set(male_n, all_alleles);
                    }

                    ++male_n;
                }

                ++indiv_n;
            }

            ++field_n;
        }

        if (haplotype.count() < MALE_MAX) { // Haplotype is discarded if too few males diverge from females (no sex specific signal at all)
            haplotype.flip();
            haplotypes[locus_n] = haplotype; // Inverts 0 and 1 in haplotype, used for comparison with males laters
            ++locus_n;
        }
    }

    haplotype_file.close();
}




const uint32_t filter_haplotypes(const std::bitset<BIT_SIZE>* haplotypes, const std::bitset<BIT_SIZE>& males, const int margin, const int n_haplotypes) {

    uint32_t loci_count = 0;
    int res = 0;

    for (int i = 0; i < n_haplotypes; ++i) {
        res = (haplotypes[i] ^ males).count();
        if (res > margin) ++loci_count;
//        if (res > margin) std::cout << i << std::endl;
//        if (i == 0) {
//            for (int j=0; j<64; ++j) std::cout << haplotypes[i][j] << " - ";
//            std::cout << "\n";
//            for (int j=0; j<64; ++j) std::cout << males[j] << " - ";
//            std::cout << "\n";
//        }
    }

    return loci_count;
}
