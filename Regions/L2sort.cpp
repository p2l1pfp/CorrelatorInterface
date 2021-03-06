#include "L2sort.h"


template<class array_t, class index_t>
void exchange(array_t a[], index_t i, index_t j) {
    #pragma HLS INLINE
  array_t t = a[i];
  a[i] = a[j];
  a[j] = t;
}

template<class array_t, class index_t, class mask_t>
void elementLoop(array_t a[], index_t j, mask_t mask) {
    #pragma HLS INLINE
 LOOP_OVER_ELEMENTS: for (int i=0; i<INPUT_SORT_SIZE; i++) {
#if (INPUT_SORT_SIZE>=256)
#pragma HLS UNROLL skip_exit_check //factor=32
#endif
    int ij=i^j;
    ap_int<K> ai = ap_int<K>(a[i].range(K-1,0));
    ap_int<K> aij = ap_int<K>(a[ij].range(K-1,0));
    if ((ij)>i) {
      if ((i&mask)==0 && ai > aij) 
	exchange(a,i,ij);
      if ((i&mask)!=0 && ai < aij)
	exchange(a,i,ij);
    }
    //select(a,i,j,mask);
  }
}

void BitonicSort(ap_uint<M> work_array[INPUT_SORT_SIZE], ap_uint<M> data_out[OUT_OBJECT_COUNT]) {
    // HLS csynth says it can't meeting timing requirements if II<4 for N=16
    // HLS csynth says it can't meeting timing requirements if II<5 for N=32
    // HLS csynth says it can't meeting timing requirements if II<7 for N=64
    // HLS csynth says it can't meeting timing requirements if II<10 for N=128
    //#pragma HLS INTERFACE ap_none port=work_array
    //#pragma HLS INTERFACE ap_none port=data_out	
    #pragma HLS ARRAY_PARTITION variable=work_array complete dim=0
    #pragma HLS ARRAY_PARTITION variable=data_out complete dim=0
    #pragma HLS PIPELINE II=SORT_II
    int j = 0;

    LOOP2: for (j=1; j>0; j=j>>1) {
        elementLoop(work_array,j,2);
    }

    LOOP4: for (j=2; j>0; j=j>>1) {
        elementLoop(work_array,j,4);
    }

    LOOP8: for (j=4; j>0; j=j>>1) {
        elementLoop(work_array,j,8);
    }

    LOOP16: for (j=8; j>0; j=j>>1) {
        elementLoop(work_array,j,16);
    }

#if (INPUT_SORT_SIZE>=32)
    LOOP32: for (j=16; j>0; j=j>>1) {
        elementLoop(work_array,j,32);
    }
#if (INPUT_SORT_SIZE>=64)
    LOOP64: for (j=32; j>0; j=j>>1) {
        elementLoop(work_array,j,64);
    }
#if (INPUT_SORT_SIZE>=128)
    LOOP128: for (j=64; j>0; j=j>>1) {
        elementLoop(work_array,j,128);
    }
#if (INPUT_SORT_SIZE>=256)
    LOOP256: for (j=128; j>0; j=j>>1) {
        elementLoop(work_array,j,256);
    }
#endif
#endif
#endif
#endif
    
    for ( int i=0; i < OUT_OBJECT_COUNT; ++i) {
#pragma HLS UNROLL
      data_out[i] = work_array[INPUT_SORT_SIZE-i-1];
      }


}

void DylanSort(ap_uint<M> data_tmp[INPUT_SORT_SIZE], ap_uint<M> data_out[OUT_OBJECT_COUNT])
{
#pragma HLS PIPELINE II=SORT_II
  for ( int i=0; i < OUT_OBJECT_COUNT; ++i) {
    ap_int<K> max = -1*(1<<(K-1)); // smallest 16-bit integer
    int maxj = -1;
    for( int j=i; j < INPUT_SORT_SIZE; ++j)
      {
#pragma HLS UNROLL
	ap_int<K> tmppt = ap_int<K>(data_tmp[j].range(K-1,0));
	if(tmppt > max) 
	  { max = tmppt; maxj = j; }
      }
    data_out[i] = data_tmp[maxj];
    data_tmp[maxj] = data_tmp[i];
  }
}

void NhanSort(ap_uint<M> data_tmp[INPUT_SORT_SIZE], ap_uint<M> data_out[OUT_OBJECT_COUNT])
{
  //#pragma HLS INTERFACE ap_none port=data_tmp
  //#pragma HLS INTERFACE ap_none port=data_out	
	#pragma HLS ARRAY_PARTITION variable=data_tmp complete dim=0
	#pragma HLS ARRAY_PARTITION variable=data_out complete dim=0
	#pragma HLS PIPELINE II=SORT_II

	ap_int<8> in_index[OUT_OBJECT_COUNT]; 
	#pragma HLS ARRAY_PARTITION variable=in_index complete dim=0

	for( int j=0; j < OUT_OBJECT_COUNT; ++j) {
		in_index[j] = -1;
	}	

	int i=0;
	for( int j=0; j < INPUT_SORT_SIZE; ++j) {
	// #pragma HLS UNROLL
		if (i>OUT_OBJECT_COUNT) continue;  		
    	ap_int<K> tmppt = ap_int<K>(data_tmp[j].range(K-1,0));
    	if (tmppt > 0) {
      		// data_out[i] = data_tmp[j];
      		in_index[i] = j;
      		i++;
    	}
  	}	

	for( int j=0; j < OUT_OBJECT_COUNT; ++j) {
		data_out[j] = data_tmp[in_index[j]];
	}
}

void RyanSort(ap_uint<M> data_tmp[INPUT_SORT_SIZE], ap_uint<M> data_out[OUT_OBJECT_COUNT])
{
#pragma HLS PIPELINE II=SORT_II
  ap_uint<8> data_geq[INPUT_SORT_SIZE];
#pragma HLS ARRAY_PARTITION variable=data_geq complete dim=0

  for( int i=0; i < INPUT_SORT_SIZE; ++i) {
#pragma HLS UNROLL
    data_geq[i] = 0;
    ap_int<K> tmppt_i = ap_int<K>(data_tmp[i].range(K-1,0));
    for( int j=0; j < INPUT_SORT_SIZE; ++j) {
#pragma HLS UNROLL
      ap_int<K> tmppt_j = ap_int<K>(data_tmp[j].range(K-1,0));
      if (tmppt_i >= tmppt_j) {
	data_geq[i] += 1;
      }
    }
  }
  int j=0;
  for( int i=0; i < INPUT_SORT_SIZE; ++i) {
#pragma HLS UNROLL
    if (data_geq[i]>(INPUT_SORT_SIZE-OUT_OBJECT_COUNT))  {
      data_out[j] = data_tmp[i];
      j++;
    }
    if (j>OUT_OBJECT_COUNT) break;
  }
}

void L2sort(ap_uint<M> data_in[NLINKS][NOBJ_PER_LINK], ap_uint<M> data_out[OUT_OBJECT_COUNT])
{

#pragma HLS INTERFACE ap_none port=data_in
#pragma HLS INTERFACE ap_none port=data_out
#pragma HLS ARRAY_PARTITION variable=data_in complete dim=0
#pragma HLS ARRAY_PARTITION variable=data_out complete dim=0
#pragma HLS PIPELINE II=SORT_II

    ap_uint<M> data_tmp[INPUT_SORT_SIZE];
#pragma HLS ARRAY_PARTITION variable=data_tmp complete dim=0

    for ( int i=0; i < NLINKS; ++i) {
#pragma HLS UNROLL
        for ( int io=0; io < NOBJ_PER_LINK; ++io) {
#pragma HLS UNROLL
	  data_tmp[i*N+io] = data_in[i][io];
        }
    }
    for ( int i=IN_OBJECT_COUNT; i < INPUT_SORT_SIZE; ++i) {
#pragma HLS UNROLL
      data_tmp[i] = 0;
    }

    //DylanSort(data_tmp, data_out);
    //BitonicSort(data_tmp, data_out);
    NhanSort(data_tmp, data_out);
    //RyanSort(data_tmp, data_out);
}
