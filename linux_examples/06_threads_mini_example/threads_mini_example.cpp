/// Threads - mini example
///
/// Simple example showing how to use threads in C++ using
/// the Standard library
///
/// Note: for compiling with g++ type in:
///
///   g++ -pthread threads_mini_example.cpp -o threads_mini_example.out
///
/// ---
/// by Prof. Dr.-Ing. J�rgen Brauer, www.juergenbrauer.org

#include <iostream> // for std::cout
#include <thread>   // for std:thread
#include <math.h>   // for math::sin

using namespace std;


// computes i^3 for different numbers i
void f1()
{
  cout << "f1 function start" << endl;
  for (int i = 1; i <= 10; i++)
  {
    printf("f1: loop i=%d \t--> %d^3 = %d\n", i, i, i*i*i);     
    std::chrono::milliseconds duration( 500 );
    this_thread::sleep_for(duration);
  }
  cout << "f1 function end" << endl;
} // f1


// computes sin(i) for different numbers start - start+10
void f2(int start)
{
  cout << "f2 function start" << endl;
  for (int i = start; i < start+10; i++)
  {
    printf("f2: loop i=%d \t--> sin(%d) = %.1f\n", i, i, sin(i));
    std::chrono::milliseconds duration( 1500 );
    this_thread::sleep_for(duration);
  }
  cout << "f2 function end" << endl;
} // f2


int main()
{
  std::thread t1(f1);
  std::thread t2(f2, 10);
  //std::thread t3(f2, 100);

  cout << "Waiting for t1 and t2 to finish (to join with main thread)..." << endl;
  t1.join();
  t2.join();

  cout << "End of threads example reached." << endl;  
}