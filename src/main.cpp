#include "arg_parser.h"
#include "bootstrap.h"

int main(int argc, char *argv[]) {

    ArgParser cmd_options(argc, argv);

    // Options: flag -> [default, type, help message]
    std::map<std::string, std::vector<std::string>> const options { {"-h", {"0", "bool", "prints this message"} },
                                                                    {"-f", {"", "string", "path to a stacks haplotypes file"} },
                                                                    {"-n", {"0", "int", "maximum number of neomales"} },
                                                                    {"-t", {"1", "int", "number of threads"} },
                                                                    {"-o", {"stdout", "string", "output file"} }
                                                                  };

    if (cmd_options.contains("-h")) {

        std::cout << std::endl << "Usage: radseq_bootstrap [options] -f input_file" << std::endl;
        std::cout << std::endl << "Options:" << std::endl << std::endl;
        for (auto o: options) std::cout << "\t" << o.first << " <" << o.second[1] << ">  " << o.second[2] << "  [" << o.second[0] << "]" << std::endl;
        std::cout << std::endl;
        return 1;
    }

    if (!cmd_options.contains("-f")){

        std::cout << std::endl << "** Error: no input file specified" << std::endl;
        std::cout << std::endl << "Usage: radseq_bootstrap [options] -f input_file" << std::endl;
        std::cout << std::endl << "Options:" << std::endl << std::endl;
        for (auto o: options) if (o.first != "-f") std::cout << "\t" << o.first << " <" << o.second[1] << ">  " << o.second[2] << "  [" << o.second[0] << "]" << std::endl;
        std::cout << std::endl;
        return 2;
    }

    const std::string file_path = cmd_options.set_value(std::string("-f"), options);

    const int max_neomales = std::stoi(cmd_options.set_value(std::string("-n"), options));
    const int n_threads = std::stoi(cmd_options.set_value(std::string("-t"), options));

    int numbers[2] {0, 0};

    const int n_col = get_numbers(file_path, numbers);
    const int n_indiv = numbers[0] + numbers[1];

    bool indiv_sexes[n_indiv];
    bool indiv_col[n_col];

    get_individual_data(file_path, indiv_sexes, indiv_col);

    std::cout << "Individual numbers : ";
    std::cout << numbers[0] << ", " << numbers[1] << std::endl;

    const int margin = numbers[0] - 1;
    const int margin_f = 1;

    std::cout << "Margins : male -> " << margin << " | female -> " << margin_f << std::endl;

    const int n_haplotypes = number_of_haplotypes(file_path, indiv_col, indiv_sexes, margin_f);

    std::cout << "Haplotypes found: " << n_haplotypes << std::endl;

    std::bitset<BIT_SIZE> haplotypes[n_haplotypes];
    for (auto i=0; i<n_haplotypes; ++i){
        haplotypes[i] = std::bitset<BIT_SIZE>();
        haplotypes[i].set();
    }

    get_haplotypes(file_path, indiv_col, indiv_sexes, haplotypes, margin_f);
    bootstrap(max_neomales, numbers, n_haplotypes, haplotypes, margin, n_threads);

    return 0;
}
