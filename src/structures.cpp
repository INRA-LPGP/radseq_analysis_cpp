#include "structures.h"


void display(individual& i){

    for (auto x: i){

        std::cout << "Locus " << x.first << std::endl;

        for (auto y: x.second){

            std::cout << "Allele " << y.first << " : ";

            for (auto z: y.second) std::cout << z << '\t';

            std::cout << std::endl;

        }
    }
}


void display(extract_data& d){

    for (auto i: d){

        std::cout << i.first << std::endl;

        display(i.second);

        std::cout << std::endl;
    }
}


void display(correspondance& c){

    for (auto i: c){

        std::cout << i.first << ": " << i.second << "  |  ";

    }
}
