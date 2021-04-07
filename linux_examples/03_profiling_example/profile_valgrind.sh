#!/bin/bash

# build the program (no special flags are needed)
g++ -std=c++11 -g cpuload_jb.cpp -o cpuload_jb.out

# run the program with callgrind; generates a file profile.callgrind that can be viewed with kcachegrind
valgrind --tool=callgrind ./cpuload_jb.out

# open profile.callgrind with kcachegrind
kcachegrind profile.callgrind