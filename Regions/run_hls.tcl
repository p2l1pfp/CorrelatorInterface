open_project -reset proj

set_top L2sort

add_files L2sort.cpp
add_files -tb L2sort_tb.cpp

open_solution -reset "solution1"

set_part {xcvu9p-flgb2104-2-i}
create_clock -period 320MHz -name default

csim_design
csynth_design
#cosim_design
#export_design

quit
