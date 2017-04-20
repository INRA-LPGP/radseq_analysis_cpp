#include "bootstrap.h"
#include "haplotypes.h"
#include <cmath>

int main(int argc, char *argv[]) {

    std::string file_path = "/home/rferon/work/code/radseq_analyses_pipeline/results_m_5_n_1_M_3/batch_0.haplotypes.tsv";

    const float e = 0.1;
    const int max_neomales = 2;

    int numbers[2] {0, 0};

    int n_col = get_numbers(file_path, numbers);
    const int n_indiv = numbers[0] + numbers[1];

    bool indiv_sexes[n_indiv];
    bool indiv_col[n_col];

    get_individual_data(file_path, indiv_sexes, indiv_col);

    std::cout << "Individual numbers:" << std::endl;
    std::cout << numbers[0] << ", " << numbers[1] << std::endl;

    const int margins[4] { int(std::round(numbers[0] - e * numbers[0])), int(std::round(e * numbers[0])),
                           int(std::round(numbers[1] - e * numbers[1])), int(std::round(e * numbers[1])) };

    hap_map haplotypes = get_haplotypes(file_path, indiv_col, n_indiv);

//    for (auto h: haplotypes){

//        for (auto i=0; i<n_indiv; ++i) std::cout << h[i] << " - ";
//        std::cout << std::endl;
//    }

    std::vector<uint32_t> loci_n = bootstrap(max_neomales, indiv_sexes, numbers, n_indiv, haplotypes, margins);

    for (auto l: loci_n) std::cout << l << " - ";
    std::cout<<std::endl;

    return 0;
}
