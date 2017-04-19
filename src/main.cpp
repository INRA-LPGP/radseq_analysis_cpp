#include "bootstrap.h"
#include "haplotypes.h"
#include <cmath>

int main(int argc, char *argv[]) {

    std::string file_path = "/home/rferon/work/code/radseq_analyses_pipeline/results_m_5_n_1_M_3/batch_0.haplotypes.tsv";

    float e = 0.1;

    uint16_t numbers[2] {0, 0};
    indiv_map individuals = get_individual_data(file_path, numbers);

    // Convert to hash map for faster access ?
    table margins {
                   {uint16_t(std::round(numbers[0] - e * numbers[0])), uint16_t(std::round(e * numbers[0]))},
                   {uint16_t(std::round(numbers[1] - e * numbers[1])), uint16_t(std::round(e * numbers[1]))}
                  };

    hap_map haplotypes = get_haplotypes(file_path, individuals);

    return 0;
}
