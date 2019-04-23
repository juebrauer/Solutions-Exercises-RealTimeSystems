/// file: memory_allocation_time_statistics.cpp
///
/// for real-time systems it is bad if memory allocation
/// times are hard to estimate in advance, i.e., are "non-deterministic"
///
/// this simple simulation allocates and frees memory blocks of random
/// sizes and measures how long each allocation and freeing takes.
///
/// the block sizes with their corresponding allocation and freeing times
/// are written to a text file for plotting as a graph.
///
/// ---
/// by Prof. Dr. Jürgen Brauer, www.juergenbrauer.org

#include <stdio.h> 
#include <conio.h> 
#include <time.h> 
#include <fstream>
#include <iostream>
using namespace std;

#define NR_SAMPLES 5000

int main() {
    
  clock_t tic, toc;
  double result = 0;
  printf("\nPress a key to start!\n\n");
  _getch();
  printf("Test started...\n\n");
  ofstream o1( "alloc_and_free_times.csv" );
  
  for (int test_nr = 0; test_nr < NR_SAMPLES; test_nr++) {

    // from time to time show that the program has not hang up
    if (test_nr % 100 == 0)
      cout << "test_nr = " << test_nr << endl;

    // guess some random size for the memory block
    int size_in_mb = (rand() % 4000) + 1; // 1-4000 MB

    // measure how long it needs to allocate that memory
    tic = clock();    
    int* ptr = (int*)malloc(size_in_mb * 1024 * 1024);
    toc = clock();    
    int duration_malloc = (int)round( ((double)(toc - tic) / (double)CLOCKS_PER_SEC)*1000.0 ); // time is now in milliseconds
    if (ptr == NULL)
    {
      cout << "!memory allocation error!" << endl;
    }

    // measure how long it needs to free that memory
    tic = clock();
    free( ptr );
    toc = clock();
    int duration_free = (int)round(((double)(toc - tic) / (double)CLOCKS_PER_SEC)*1000.0); // time is now in milliseconds

    // write block size, allocation, free time to file
    o1 << size_in_mb << " " << duration_malloc << " " << duration_free << endl;
  }
  o1.close();  
  printf("\nTest finished!\n");
  printf("\nPress a key to exit.\n");
  _getch(); 

} // end main 