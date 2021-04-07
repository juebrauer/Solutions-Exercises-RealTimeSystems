#!/bin/bash

# build the program with profiling support (-gp)
g++ -std=c++11 -pg cpuload_jb.cpp -o cpuload_jb.out

# run the program; generates the profiling data file (gmon.out)
./cpuload_jb.out

# print the callgraph to stdout
gprof --graph --brief cpuload_jb.out