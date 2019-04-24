#include "L2sort.h"

void L2sort(ap_uint<64> data_in[NLINKS][NOBJ_PER_LINK], ap_uint<64> data_out[OUT_OBJECT_COUNT])
{

#pragma HLS INTERFACE ap_none port=data_in
#pragma HLS INTERFACE ap_none port=data_out
#pragma HLS INTERFACE ap_none port=return

#pragma HLS ARRAY_PARTITION variable=data_in  complete dim=0
#pragma HLS ARRAY_PARTITION variable=data_out complete dim=0
#pragma HLS PIPELINE II=1
//#pragma HLS LATENCY min=19 max=19

    ap_uint<64> data_tmp[IN_OBJECT_COUNT];
#pragma HLS ARRAY_PARTITION variable=data_tmp complete dim=0

    for ( int i=0; i < NLINKS; ++i) {
#pragma HLS UNROLL
        for ( int io=0; io < NOBJ_PER_LINK; ++io) {
#pragma HLS UNROLL
            data_tmp[i*NOBJ_PER_LINK+io] = data_in[i][io];
        }
    }

    for ( int i=0; i < OUT_OBJECT_COUNT; ++i) {
        ap_int<16> max = -1*(1<<14);
        int maxj = -1;
        for( int j=i; j < IN_OBJECT_COUNT; ++j)
        {
#pragma HLS UNROLL
            ap_int<16> tmppt = ap_int<16>(data_tmp[j].range(15,0));
            if(tmppt > max) 
                { max = tmppt; maxj = j; }
        }
        data_out[i] = data_tmp[maxj];
        data_tmp[maxj] = data_tmp[i];
    }

}
