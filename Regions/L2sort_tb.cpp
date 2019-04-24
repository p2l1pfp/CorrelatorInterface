#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include "L2sort.h"

using namespace std;

int main(int argc, char ** argv) {

    std::cout<<"Starting test..."<<std::endl;

    ap_uint<M> data_in[NLINKS][NOBJ_PER_LINK];
    ap_uint<M> data_out[OUT_OBJECT_COUNT];

    int sign = -1;

    for (int i = 0; i < NLINKS; i++) {
        for (int io = 0; io < NOBJ_PER_LINK; io++) {
            sign = sign*-1;
            data_in[i][io].range(K-1,0) = ap_int<K>(1000+4*i*sign+3*io).range(15,0);
            std::cout<<ap_int<K>(data_in[i][io].range(K-1,0))<<"\t";
        }
    }
    std::cout<<std::endl;
    std::cout<<std::endl;
    
    L2sort(data_in,data_out);

    for (int i = 0; i < OUT_OBJECT_COUNT; i++) {
        std::cout<<ap_int<K>(data_out[i].range(K-1,0))<<"\t";
    }
    std::cout<<std::endl;

}
