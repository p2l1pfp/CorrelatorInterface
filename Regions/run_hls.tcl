open_project -reset proj_justcsim

set nLinks 10
set nObjPerLink 18
set inObjectCount 180
set inputSortSize 256
set outObjectCount 30

set_top L2sort

add_files L2sort.cpp -cflags "-DINPUT_SORT_SIZE=${inputSortSize} -DIN_OBJECT_COUNT=${inObjectCount} -DOUT_OBJECT_COUNT=${outObjectCount} -DNLINKS=${nLinks} -DNOBJ_PER_LINK=${nObjPerLink}"
add_files -tb L2sort_tb.cpp -cflags "-DINPUT_SORT_SIZE=${inputSortSize} -DIN_OBJECT_COUNT=${inObjectCount} -DOUT_OBJECT_COUNT=${outObjectCount} -DNLINKS=${nLinks} -DNOBJ_PER_LINK=${nObjPerLink}"

open_solution -reset "solution1"

set_part {xcvu9p-flgb2104-2-i}
create_clock -period 320MHz -name default

csim_design
csynth_design
#cosim_design
#export_design

quit
