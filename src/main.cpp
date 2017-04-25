#include "bootstrap.h"
#include "haplotypes.h"
#include <cmath>

int main(int argc, char *argv[]) {

    std::string file_path = argv[1];

    const float e = 0.95;
    const int max_neomales = std::stoi(argv[2]);
    const int n_threads = std::stoi(argv[3]);

    int numbers[2] {0, 0};

    int n_col = get_numbers(file_path, numbers);
    const int n_indiv = numbers[0] + numbers[1];

    bool indiv_sexes[n_indiv];
    bool indiv_col[n_col];

    get_individual_data(file_path, indiv_sexes, indiv_col);

    std::cout << "Individual numbers:" << std::endl;
    std::cout << numbers[0] << ", " << numbers[1] << std::endl;

    const int margin = int(std::round(numbers[0] * e));
    const int margin_f = int(std::round(numbers[1] * (1 - e)));
    std::cout << "Margin : " << margin << std::endl;

    int n_haplotypes = number_of_haplotypes(file_path, indiv_col, indiv_sexes, margin_f);

    std::cout << "Haplotypes found: " << n_haplotypes << std::endl;

    std::bitset<BIT_SIZE> haplotypes[n_haplotypes];
    for (auto i=0; i<n_haplotypes; ++i){
        haplotypes[i] = std::bitset<BIT_SIZE>();
        haplotypes[i].set();
    }

    std::cout << "SIZE : " << sizeof(haplotypes[0]) << std::endl;

    get_haplotypes(file_path, indiv_col, indiv_sexes, haplotypes, margin_f);

//        for (auto i=0; i<n_haplotypes; ++i){
//            for (auto j=0; j<n_indiv; ++j) std::cout << haplotypes[i][j] << " - ";
//            std::cout << std::endl;
//        }

//    int n_males = numbers[0];
//    bool males[n_males];
//    for (auto i=0; i<n_males; ++i) males[i] = true;
//    auto loci_n = filter_haplotypes(haplotypes, males, margin, n_males, n_haplotypes);
//    std::cout << "Loci : " << loci_n << std::endl;

    auto o = bootstrap(max_neomales, numbers, n_haplotypes, haplotypes, margin, n_threads);

    return 0;
}
