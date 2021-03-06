open_project -reset proj_II1_obj36_20_nhansort

set nLinks 2 
set nObjPerLink 18
set inObjectCount 36
set inputSortSize 64
set outObjectCount 20
set sortII 1
set_top L2sort

add_files L2sort.cpp -cflags "-DINPUT_SORT_SIZE=${inputSortSize} -DIN_OBJECT_COUNT=${inObjectCount} -DOUT_OBJECT_COUNT=${outObjectCount} -DNLINKS=${nLinks} -DNOBJ_PER_LINK=${nObjPerLink} -DSORT_II=${sortII}"
add_files -tb L2sort_tb.cpp -cflags "-DINPUT_SORT_SIZE=${inputSortSize} -DIN_OBJECT_COUNT=${inObjectCount} -DOUT_OBJECT_COUNT=${outObjectCount} -DNLINKS=${nLinks} -DNOBJ_PER_LINK=${nObjPerLink} -DSORT_II=${sortII}"

open_solution -reset "solution1"

config_array_partition -maximum_size 32400
set_part {xcvu9p-flgb2104-2-i}
create_clock -period 320MHz -name default

csim_design
csynth_design
#cosim_design
#export_design

quit
