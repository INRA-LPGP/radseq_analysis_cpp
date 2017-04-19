#include "structures.h"

void visualize(indiv_map i){

    for (auto x: i) {
        std::cout << x.first << " : " << x.second.name << ", " << x.second.male << "\n";
    }
}


//void visualize(hap_map h){

//    for (auto x: h) {

//        std::cout << x.first << " : ";
//        for (auto i: x.second) std::cout << i << "  |  ";
//        std::cout << std::endl;
//    }
//}

//void visualize(hap_map h, uint32_t locus){

//    std::cout << locus << " : ";
//    for (auto i: h[locus]) std::cout << i << "  |  ";
//    std::cout << std::endl;
//}


