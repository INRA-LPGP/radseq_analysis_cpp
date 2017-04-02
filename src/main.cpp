#include <iostream>
#include "gzstream.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <array>
#include <vector>
#include <map>


typedef std::map<std::string, std::vector<std::string>> alleles_struct;
typedef std::map<std::string, alleles_struct> loci_struct;

std::vector<std::string>::iterator process_line(std::string& line, std::vector<std::string>& markers, loci_struct& loci){

    uint tab_count = 0;
    std::vector<std::string> tabs;
    std::string word = "";
    std::vector<std::string>::iterator pos = markers.end();

    for (auto c: line) {

        if (c != '\t' and c != '\n') {

            word += c;

        } else {

            if (tab_count == 1 or tab_count == 2 or tab_count == 6 or tab_count == 7 or tab_count == 9){

                if (tab_count == 2){
                    pos = std::find(markers.begin(), markers.end(), word);
                    if (pos == markers.end()) return pos;
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



int main(int argc, char *argv[]) {

    std::string path = "/home/romain/work/results_10/a_melas_F1.tags.tsv.gz";

    igzstream in(path.c_str());
    std::string line;
    std::vector<std::string> markers = {"1523", "12658", "8695", "13", "546"};
    loci_struct loci;

    for (auto m:markers) loci[m] = alleles_struct();

    std::vector<std::string>::iterator current_locus = markers.end();
    std::vector<std::string>::iterator temp_locus = markers.begin();

    std::getline(in, line);

    while(std::getline(in, line) and markers.size() > 0){
        temp_locus = process_line(line, markers, loci);
        if (temp_locus != markers.end() and current_locus == markers.end()) current_locus = temp_locus;
        if (temp_locus != current_locus){
            markers.erase(current_locus);
            current_locus = markers.end();
        }
    }

    for (loci_struct::iterator it=loci.begin(); it!=loci.end(); ++it) {
        std::cout << "Locus " << it->first << std::endl;
        for (alleles_struct::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            std::cout << "Allele " << it2->first << " : ";
            for(auto w:it2->second){
                std::cout << w << '\t';
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
