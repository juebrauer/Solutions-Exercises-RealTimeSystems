#!/bin/bash

# build the program; For our demo program, we specify -DWITHGPERFTOOLS to enable the gperftools specific #ifdefs
g++ -std=c++11 -DWITHGPERFTOOLS -lprofiler -g cpuload_jb.cpp -o cpuload_jb

# run the program; generates the profiling data file (profile.log in our example)
./cpuload_jb

# convert profile.log to callgrind compatible format
pprof --callgrind ./cpuload_jb profile.log > profile.callgrind

# open profile.callgrind with kcachegrind
kcachegrind profile.callgrind
