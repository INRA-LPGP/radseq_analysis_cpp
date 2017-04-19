#include "process_file.h"
#include "loci_to_extract.h"

int main(int argc, char *argv[]) {

    std::string data_path = "/home/rferon/work/analyses/multi_species/rad_seq/denovo_map/data/tags/ameirus_melas/";
    std::string file_name = "a_melas_M10_10.tags.tsv.gz";

    std::string loci_file_path = "/home/rferon/work/analyses/multi_species/rad_seq/denovo_map/results/batch_1/species/a_melas/a_melas_10_sex_specific.txt";

    std::string file_path = data_path + "/" + file_name;

    extract_data loci_to_extract = read_loci_file(loci_file_path);

    process_file(file_path, loci_to_extract);

    return 0;
}
