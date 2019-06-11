/// Threads (5) - Deadlocks
///
/// The following code shall show you what a deadlock
/// in the context of threads is.
///
/// For two threads T1 and T2, where each thread needs access to two
/// ressources R1 and R2, we can accidently come to a situation
/// in which e.g.
/// T1 already holds ressource R1 and needs access to R2
/// but
/// T2 already holds ressource R2 and needs access to R1
///
/// So both threads will sleep() till the other ressource is free
/// again, but this will never happen.
/// This situation is called a deadlock.
///
///
/// note:
///  in T2() try the order
///    mu1.lock(); mu2.lock(); // no deadlock
///  then try the order
///    mu2.lock(); mu1.lock(); // deadlock!
///  
///
/// ---
/// by Prof. Dr. Jürgen Brauer, www.juergenbrauer.org

#include <iostream> // for std::cout
#include <conio.h>  // for _getch()
#include <thread>   // for std:thread
#include <mutex>    // for std::mutex
#include <vector>   // for std::vector

using namespace std;

mutex R1;
mutex R2;

// Shared ressources (shared memory):
vector<int> v1;
vector<int> v2;

const int N = 75;


void T1()
{
  for (int i = 0; i < N; i++)
  {
    printf("T1: Trying to acquire resource R1...\n");
    R1.lock();
    printf("T1: got resource R1!\n");

       printf("T1: Trying to acquire resource R2...\n");
       R2.lock();
       printf("T1: got resource R2!\n");

          printf("T1: storing i=%d and i*i=%d\n", i, i*i);
          v1.push_back(i);
          v2.push_back(i*i);

       printf("T1: Releasing resource R2...\n");
       R2.unlock();

    printf("T1: Releasing resource R1...\n");
    R1.unlock();

    //std::chrono::milliseconds duration(1);
    //this_thread::sleep_for(duration);
  }
} // T1


void T2()
{
   for (int i = 0; i < N; i++)
   {
      printf("T2: Trying to acquire resource R2...\n");
      R2.lock();
      printf("T2: got resource R2!\n");

         printf("T2: Trying to acquire resource R1...\n");
         R1.lock();
         printf("T2: got resource R1!\n");
      
            printf("T2: storing i=%d and i*i=%d\n", i, i*i);
            v1.push_back(i);
            v2.push_back(i*i);

         printf("T2: Releasing resource R1...\n");
         R1.unlock();

      printf("T2: Releasing resource R2...\n");
      R2.unlock();

      //std::chrono::milliseconds duration(1);
      //this_thread::sleep_for(duration);
   }
} // T2


int main()
{
  const int nr_threads = 2;
  std::thread* my_threads[nr_threads];
  my_threads[0] = new thread(T1);
  my_threads[1] = new thread(T2);
    
  cout << "Waiting for all threads to finish ...\n\n";
  for (int i = 0; i < nr_threads; i++)
    my_threads[i]->join();

  cout << "End of deadlock demo reached." << endl;
  _getch();
}