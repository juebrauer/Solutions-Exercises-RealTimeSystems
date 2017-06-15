/// Threads (3) - Mutex
///
/// Example for two threads that share some
/// memory (f2_called_counter).
/// Access to this shared memory is protected
/// using a mutex.
///
/// Note: first start program and observe
///       whether f2_called_counter
///       is 50 as expected!
///       
///       Then try it without the
///         mtx.lock() / mtx.unlock()
///       lines! Try it several times
///       and observe whether f2_called_counter
///       is still 50 as expected.
///
/// ---
/// by Prof. Dr.-Ing. Jürgen Brauer, www.juergenbrauer.org

#include <iostream> // for std::cout
#include <conio.h>  // for _getch()
#include <thread>   // for std:thread
#include <mutex>    // for std::mutex

using namespace std;

// mutex for critical section
std::mutex mtx;

int f2_called_counter = 0;


/// computes i^3 for given i
int f2(int i)
{
  mtx.lock();
  int new_counter_value = f2_called_counter;
  new_counter_value = new_counter_value + 1; // at this point t1 might be interrupted by t2!
  f2_called_counter = new_counter_value;
  mtx.unlock();

  return i*i*i;

} // f2


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
  const int nr_threads = 5;
  std::thread* my_threads[nr_threads];
  for (int i = 0; i < nr_threads; i++)
    my_threads[i] = new std::thread(f1);

  printf("f2_called_counter = %d\n", f2_called_counter);

  cout << "Waiting for all threads to finish ..." << endl;
  for (int i = 0; i < nr_threads; i++)
    my_threads[i]->join();

  printf("f2_called_counter = %d\n", f2_called_counter);

  cout << "End of threads example reached." << endl;
  _getch();
}