#include "bootstrap.h"
#include "haplotypes.h"
#include <cmath>

int main(int argc, char *argv[]) {

    std::string file_path = "/home/rferon/work/code/radseq_analyses_pipeline/results_m_5_n_1_M_3/batch_0.haplotypes.tsv";

    const float e = 0.95;
    const int max_neomales = 3;

    int numbers[2] {0, 0};

    int n_col = get_numbers(file_path, numbers);
    const int n_indiv = numbers[0] + numbers[1];

    bool indiv_sexes[n_indiv];
    bool indiv_col[n_col];

    get_individual_data(file_path, indiv_sexes, indiv_col);

    std::cout << "Individual numbers:" << std::endl;
    std::cout << numbers[0] << ", " << numbers[1] << std::endl;

    const int margin = int(std::round(numbers[0] * e));
    std::cout << "Margin : " << margin << std::endl;

    int n_haplotypes = number_of_haplotypes(file_path);

    bool** haplotypes;
    haplotypes = new bool* [n_haplotypes];
    for (auto i=0; i<n_haplotypes; ++i) haplotypes[i] = new bool[numbers[0]];

    get_haplotypes(file_path, indiv_col, haplotypes);

//    for (auto i=0; i<n_haplotypes; ++i){
//        for (auto j=0; j<n_indiv; ++j) std::cout << haplotypes[i][j] << " - ";
//        std::cout << std::endl;
//    }

    std::cout << "Haplotypes found: " << n_haplotypes << std::endl;

//    auto loci_n = filter_haplotypes(haplotypes, indiv_sexes, margin, n_indiv, n_haplotypes);
//    std::cout << loci_n << std::endl;
    std::vector<uint32_t> loci_n = bootstrap(max_neomales, indiv_sexes, numbers, numbers[0], n_haplotypes, haplotypes, margin);

    for (auto l: loci_n) std::cout << l << " - ";
    std::cout<<std::endl;

    for (auto i=0; i<n_haplotypes; ++i) delete[] haplotypes[i];
    delete[] haplotypes;

    return 0;
}
