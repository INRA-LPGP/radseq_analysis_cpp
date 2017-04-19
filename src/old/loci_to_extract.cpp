#include "loci_to_extract.h"

extract_data read_loci_file(std::string file_path){

    std::ifstream file;
    file.open(file_path.c_str());
    extract_data loci_to_extract; // map: individual -> vector of correspondances
    std::vector<std::string> line_tabs;
    std::vector<std::string> individuals_tabs;

    std::string line;

    while(std::getline(file, line)){

        if (line != "\n"){

            line_tabs = split(line, "\t");

            if (line_tabs.size() == 2) {

                individuals_tabs = split(line_tabs[1], ",");

                for (auto f: individuals_tabs){

                    std::string individual = "", locus = "";
                    bool id = true;

                    for (auto c: f){ // split a field with separator "_" (left -> individual ID, right -> locus correspondance with catalog)

                        if (c == '_'){

                            id = false;

                        } else if (c != '\n') {

                            if (id) individual += c; else locus += c;

                        }
                    }

                    loci_to_extract[individual][locus] = line_tabs[0]; // line_tabs[0] contains the catalog locus ID
                }

            } else {

                if (line_tabs.size() < 2) throw std::range_error("Not enough fields in line : " + line);
                if (line_tabs.size() > 2) throw std::range_error("Too many fields in line : " + line);

            }
        }
    }

    return loci_to_extract;
}
