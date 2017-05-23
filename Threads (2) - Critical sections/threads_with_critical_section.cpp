/// Threads (2) - Critical sections 
///
/// Example of two threads with critical section
/// in C++ using the Standard library
///
/// Critcial section here is incrementing
/// and writing f2_called_counter
///
/// Two threads are generated.
/// Each thread executes a for-loop 10 times in f1()
/// and calls function f2()
///
/// We keep track of how often f2() has been called
/// by the use of f2_called_counter.
///
/// f2_called_counter should be 20 at the end of the
/// program, should'nt it?
///
/// Note: start program several times
///       and observe whether f2_called_counter
///       is 20 as expected!
///
/// ---
/// by Prof. Dr.-Ing. Jürgen Brauer, www.juergenbrauer.org

#include <iostream> // for std::cout
#include <conio.h>  // for _getch()
#include <thread>   // for std:thread

using namespace std;

int f2_called_counter = 0;  // this is a small piece of shared memory


/// computes i^3 for given i
int f2(int i)
{
  int new_counter_value = f2_called_counter;
  new_counter_value = new_counter_value + 1; // at this point t1 might be interrupted by t2!
  f2_called_counter = new_counter_value;

  return i*i*i;
}


/// show what i^3 is for i=0,1,2,...,9
void f1()
{
  cout << "f1 function start" << endl;
  for (int i = 0; i < 10; i++)
  {
    std::thread::id this_id = std::this_thread::get_id();
    printf("f1: (thread id=%d) loop i=%d \t--> %d^3 = %d\n", 
      this_id, i, i, f2(i));
    std::chrono::milliseconds duration(10);
    this_thread::sleep_for(duration);
  }
  cout << "f1 function end" << endl;
} // f1


int main()
{
  std::thread t1(f1);
  std::thread t2(f1);

  printf("f2_called_counter = %d\n", f2_called_counter);

  cout << "Waiting for t1 to finish ..." << endl;
  t1.join();

  cout << "Waiting for t2 to finish ..." << endl;
  t2.join();

  printf("f2_called_counter = %d\n", f2_called_counter);

  cout << "End of threads example reached." << endl;
  _getch();
}