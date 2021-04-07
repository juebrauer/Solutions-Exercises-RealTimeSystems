#!/bin/bash

# build the program
g++ -std=c++11 -g cpuload_jb.cpp -o cpuload_jb

# run the program; generates the profiling data file (perf.data)
perf record -g ./cpuload_jb

# analyze the profile
perf report
