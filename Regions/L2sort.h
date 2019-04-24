#ifndef SORTER_H
#define SORTER_H

#include <ap_int.h>

const unsigned int M = 64; //number of bits in each data item                                                                                      
const unsigned int K = 16; //number of (signed) bits to sort on
const unsigned int N = NOBJ_PER_LINK;

void BitonicSortOptimizedInline(ap_uint<M> work_array[INPUT_SORT_SIZE]);
void L2sort(ap_uint<M> data_in[NLINKS][NOBJ_PER_LINK], ap_uint<M> data_out[OUT_OBJECT_COUNT]);

#endif
