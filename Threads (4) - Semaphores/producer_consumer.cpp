/// Threads (4) - Producer / Consumer solution with 3 semaphores
///
/// note:
///  try out what happens without using
///  semaphore fillCount (-> hang up)
///  semaphore emptyCount (-> sm will be filled and filled ...)
///
/// ---
/// by Prof. Dr.-Ing. Jürgen Brauer, www.juergenbrauer.org

#include <iostream> // for std::cout
#include <conio.h>  // for _getch()
#include <thread>   // for std:thread
#include <mutex>    // for std::mutex
#include <deque>    // for std::deque

#include "Semaphores.h"

using namespace std;

Semaphore useQueue(1);
Semaphore fillCount(0);
Semaphore emptyCount(5);

// shared memory
deque<int> sm;

void producer()
{
  int i = 0;
  while (true)
  {
    emptyCount.P();
    useQueue.P();
    sm.push_back(i);
    printf("produced data item '%d'. List size is now %d\n",
       i, (unsigned int)sm.size());
    useQueue.V();
    fillCount.V();

    i++;
    std::chrono::milliseconds duration( (rand() % 5)*100 );
    //std::chrono::milliseconds duration(3000 + (rand() % 5) * 1000);
    this_thread::sleep_for(duration);
  }

}


void consumer()
{
  while (true)
  {
    fillCount.P();
    useQueue.P();
    int number = sm.front();
    sm.pop_front();
    printf("consumed data item '%d'. List size is now %d\n",
       number, (unsigned int)sm.size());
    useQueue.V();
    emptyCount.V();

    std::chrono::milliseconds duration(3000+(rand() % 5) * 1000);
    //std::chrono::milliseconds duration( (rand() % 5)*100 );
    this_thread::sleep_for(duration);
  }
}



int main()
{
  const int nr_threads = 3;
  std::thread* my_threads[nr_threads];
  my_threads[0] = new thread( producer );
  my_threads[1] = new thread( consumer );
  my_threads[2] = new thread( consumer );

  cout << "Waiting for all threads to finish ..." << endl;
  for (int i = 0; i < nr_threads; i++)
    my_threads[i]->join();
    
  cout << "End of producer/consumer demo reached." << endl;
  _getch();
}