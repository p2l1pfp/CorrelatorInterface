#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include "L2sort.h"

using namespace std;

int main(int argc, char ** argv) {

    std::cout<<"Starting test..."<<std::endl;

    ap_uint<64> data_in[NLINKS][NOBJ_PER_LINK];
    ap_uint<64> data_out[OUT_OBJECT_COUNT];

    int sign = -1;

    for (int i = 0; i < NLINKS; i++) {
        for (int io = 0; io < NOBJ_PER_LINK; io++) {
            sign = sign*-1;
            data_in[i][io].range(15,0) = ap_int<16>(1000+4*i*sign+3*io).range(15,0);
            std::cout<<ap_int<16>(data_in[i][io].range(15,0))<<"\t";
        }
    }
    std::cout<<std::endl;
    std::cout<<std::endl;
    
    L2sort(data_in,data_out);

    for (int i = 0; i < OUT_OBJECT_COUNT; i++) {
        std::cout<<ap_int<16>(data_out[i].range(15,0))<<"\t";
    }
    std::cout<<std::endl;

}
