#ifndef SORTER_H
#define SORTER_H

#include <ap_int.h>

#define NLINKS 10
#define NOBJ_PER_LINK 18
#define IN_OBJECT_COUNT (NLINKS * NOBJ_PER_LINK)// ... 10 links from a detector group, and 18 objects per link
#define OUT_OBJECT_COUNT 30// ... 30 objects to HLS algo per detector

void L2sort(ap_uint<64> data_in[NLINKS][NOBJ_PER_LINK], ap_uint<64> data_out[OUT_OBJECT_COUNT]);

#endif
