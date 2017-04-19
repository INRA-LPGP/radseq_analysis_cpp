#include "bootstrap.h"
#include "haplotypes.h"

int main(int argc, char *argv[]) {

    std::string file_path = "/home/rferon/work/code/radseq_analyses_pipeline/results_m_5_n_1_M_3/batch_0.haplotypes.tsv";

    rep();

    get_individual_data(file_path);

    return 0;
}
