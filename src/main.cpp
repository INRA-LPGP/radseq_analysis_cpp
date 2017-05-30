#include "arg_parser.h"
#include "bootstrap.h"

#define STDOUT "/dev/stdout"


const std::string log_file(const std::string output_path){

    if (output_path != STDOUT) {

        return output_path + std::string(".log");

    } else {

        return std::string("radseq_bootstrap.log");

    }
}

int main(int argc, char *argv[]) {

    ArgParser cmd_options(argc, argv);

    // Options: flag -> [default, type, help message]
    std::map<std::string, std::vector<std::string>> const options { {"-h", {"0", "bool", "prints this message"} },
                                                                    {"-f", {"", "string", "path to a stacks haplotypes file"} },
                                                                    {"-t", {"1", "int", "number of threads"} },
                                                                    {"-o", {"/dev/stdout", "string", "output file"} }
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
    const std::string output_path = cmd_options.set_value(std::string("-o"), options);
    const std::string log_path = log_file(output_path);

    std::ofstream log_file(log_path);
    char time[DTTMSZ];

    log_file << "Job started: " << print_time(time) << std::endl << std::endl;
    log_file << "Options: " << std::endl;
    for (auto o: options) {
        log_file << "\t" << o.first << " : " << cmd_options.set_value(std::string(o.first), options) << std::endl;
    }
    log_file << std::endl;

    const int n_threads = std::stoi(cmd_options.set_value(std::string("-t"), options));

    struct Infos infos = get_infos(file_path);

    log_file << "Number of individuals : ";
    log_file << infos.n_males << " males, " << infos.n_females << " females" << std::endl;

    const int margin = infos.n_males - 1; // -1 to be able to compare strictly in filter_haplotypes (faster comparison)
    const int margin_f = 0;

    log_file << "Margins : ";
    log_file << " Min males [" << margin + 1 << "]  | Max females [" << margin_f << "]" << std::endl;

    const std::pair<int, int> haplotypes_numbers = number_of_haplotypes(file_path, infos, margin_f);
    const int n_haplotypes = haplotypes_numbers.first;

    log_file << "Total haplotypes : " << haplotypes_numbers.second << " | Sex-variable haplotypes : " << n_haplotypes << std::endl;

    log_file.close();

    std::bitset<BIT_SIZE> haplotypes[n_haplotypes];
    for (auto i=0; i<n_haplotypes; ++i){
        haplotypes[i] = std::bitset<BIT_SIZE>();
        haplotypes[i].set();
    }

    get_haplotypes(file_path, infos, haplotypes, margin_f);

//    std::vector<int> combination {0, 3, 5, 7, 9, 10, 14, 16, 17, 18, 19, 22, 23, 24, 25, 27, 29};
////    std::vector<int> combination {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

//    std::bitset<BIT_SIZE> males;
//    males.reset();
//    for (auto c: combination) males.flip(c);

//    int res = filter_haplotypes(haplotypes, males, margin, n_haplotypes);
//    std::cout << res << std::endl;

    bootstrap(infos, n_haplotypes, haplotypes, margin, n_threads, output_path, log_path);

    return 0;
}
