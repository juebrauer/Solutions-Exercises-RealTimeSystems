/**
file: memory_allocation_time_statistics.cpp

for real-time systems it is bad if memory allocation
times are hard to estimate in advance, i.e., are "non-deterministic"

this simple simulation allocates and frees memory blocks of random
sizes and measures how long each allocation and freeing takes.

the block sizes with their corresponding allocation and freeing times
are written to a text file for plotting as a graph.

Interesting article in the context of "lazy memory allocation"
(vs. "eager memory allocation")
https://stackoverflow.com/questions/11645513/linux-kernel-memory-management-does-it-use-consecutive-memory-pages-all-the-tim

"For example, malloc(40960) could request 10 pages of memory,
 assuming 4KB page size. The Memory Management Unit (MMU)
 doesn't immediately allocate 10 pages in the physical memory.
 Instead, new entries (10 in this case) are added to the address
 space of the invoking process, that is, new entries are added
 in the page table of this process. This virtual memory will be contiguous.

 Now assume that later during the execution, the process tries
 to use this newly allocated memory, At that moment, a page fault
 is invoked because the process touched unallocated memory.
 The MMU will then allocate an actual physical page
 (4096 continuous bytes) in RAM, update the page table
 and resume execution of the process."

---
by Prof. Dr. Juergen Brauer, www.juergenbrauer.org
*/

#include <stdio.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <string.h>

#define NR_SAMPLES 2000

int main() {
    
  clock_t tic1, toc1;
  clock_t tic2, toc2;
  clock_t tic3, toc3;
  double result = 0;

  std::cout << "CLOCKS_PER_SEC=" << CLOCKS_PER_SEC << std::endl;

  std::ofstream o1( "alloc_and_free_times.csv" );
  
  for (int test_nr = 0; test_nr < NR_SAMPLES; test_nr++) {

    // 1. from time to time show that the program has not hang up
    if (test_nr % 100 == 0)
      std::cout << "test_nr = " << test_nr << std::endl;

    // 2. guess some random size for the memory block
    unsigned long long size_in_mb = (rand() % 4000) + 100; // 1-4000 MB
    unsigned long long size_in_bytes = size_in_mb * 1024 * 1024;
    std::cout << "exp: " << test_nr << 
                " --> allocation size [MB]: " << size_in_mb << " "; //std::endl;

    // 3. allocate memory    
    char* ptr = (char*)malloc(size_in_bytes);
    
    // enforce the memory allocation now by using it!
    tic1 = clock();
    memset(ptr, 0, size_in_bytes);
    toc1 = clock();
    unsigned long long duration_allocation_and_set_time = toc1-tic1;

    // now the memory should be allocated
    // and in the following we only set it
    // measure only the usage time
    tic2 = clock();
    memset(ptr, 0, size_in_bytes);
    toc2 = clock();
    unsigned long long duration_only_set_time = toc2-tic2;
    
    // 4. compute how long it took to allocate the memory
    unsigned long long duration_malloc = duration_allocation_and_set_time - duration_only_set_time;
    if (ptr == NULL)
    {
      std::cout << "!memory allocation error!" << std::endl;
    }

    std::cout << " alloc+set=" << duration_allocation_and_set_time
              << " set only=" << duration_only_set_time
              << " --> alloc only=" << duration_malloc << std::endl;

    // 5. free the memory block
    tic1 = clock();
    free( ptr );
    toc1 = clock();

    // 6. measure how long it needs to free that memory
    // duration time is then in milliseconds
    unsigned long long duration_free = toc1-tic1;

    // 7. write block size, allocation, free time to file
    o1 << size_in_mb << " " << duration_malloc << " " << duration_free << std::endl;
  }

  o1.close();

  std::cout << "Memory allocation/freeing experiment finished." << std::endl;

} // end main 