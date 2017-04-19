#include "process_file.h"


correspondance::const_iterator process_line(std::string& line, correspondance& loci_to_extract, individual& loci){

    uint tab_count = 0;
    std::vector<std::string> tabs;
    std::string word = "";
    correspondance::const_iterator pos = loci_to_extract.end();

    for (auto c: line) {

        if (c != '\t' and c != '\n') {

            word += c;

        } else {

            if (tab_count == 1 or tab_count == 2 or tab_count == 6 or tab_count == 7 or tab_count == 9){

                if (tab_count == 2){

                    pos = loci_to_extract.find(word);
                    if (pos == loci_to_extract.end()) return pos;
                }

                tabs.push_back(word);
            }

            ++tab_count;
            word = "";
        }
    }

    loci[tabs[1]][tabs[2] + tabs[3]] = tabs;
    return pos;
}



void process_file(std::string& file_path, extract_data& loci_data){

    igzstream input_file(file_path.c_str());

    std::string line;
    // Extract individual ID from first line in file, then goes back one line.
    std::getline(input_file, line);
    std::getline(input_file, line);
    std::string id = split(line, "\t")[1];
    correspondance loci_to_extract = loci_data[id];
    input_file.close(); // Sadly seek doesn't seem to work with gzstream, it's easier to close the file then reopen it ...
    input_file.open(file_path.c_str());
    std::getline(input_file, line);

    individual current_individual;
    for (auto m:loci_to_extract) current_individual[m.first] = locus(); // Create empty locus for every position
    correspondance::const_iterator current_locus = loci_to_extract.end();
    correspondance::const_iterator temp_locus = loci_to_extract.begin();


    while(std::getline(input_file, line) and not current_individual.empty()){

        temp_locus = process_line(line, loci_to_extract, current_individual);

        if (temp_locus != loci_to_extract.end() and current_locus == loci_to_extract.end()) current_locus = temp_locus;

        if (temp_locus != current_locus) {

            loci_to_extract.erase(current_locus);
            current_locus = loci_to_extract.end();

        }
    }

    display(current_individual);
}


