/// Threads - An example with a critical section
///
/// Example of two threads with a critical section
/// in C++. Threads implemented using the Standard library
///
/// Critcial section here is incrementing
/// and writing f2_called_counter
///
/// Two threads are generated.
/// Each thread executes a for-loop 10 times in f1()
/// and calls function f2().
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
/// Note: for compiling with g++ type in:
///
///   g++ -pthread thread_example_with_unprotected_critical_section.cpp -o thread_example_with_unprotected_critical_section.out
///
/// What does the "-pthread" flag mean?
/// pthread
///    Adds support for multithreading with the pthreads library. This
///    option sets flags for both the preprocessor and linker.
///
/// ---
/// by Prof. Dr.-Ing. Juergen Brauer, www.juergenbrauer.org

#include <iostream> // for std::cout
#include <thread>   // for std:thread


int f2_called_counter = 0;  // this is a small piece of shared memory


/// computes i^3 for given i
int f2(int i)
{
    int new_counter_value = f2_called_counter;

    
    // ... START of some code that works with the shared memory <f2_called_counter>...
    new_counter_value = new_counter_value + 1; // ATTENTION! At this point t1 might be interrupted by t2!
    std::chrono::milliseconds duration(rand() % 1000);
    std::this_thread::sleep_for(duration);
    // ... END of some code that works with the shared memory <f2_called_counter>...

    f2_called_counter = new_counter_value;

    return i*i*i;
}


/// calls 10x function f2()
void f1()
{
    std::cout << "f1 function start" << std::endl;
    for (int i = 0; i < 10; i++)
    {
        f2(i);        
    }
    std::cout << "f1 function end" << std::endl;
} // f1


int main()
{
    std::thread t1(f1);
    std::thread t2(f1);

    std::cout << "f2_called_counter = " << f2_called_counter << std::endl;

    std::cout << "Waiting for t1 to finish ..." << std::endl;
    t1.join();

    std::cout << "Waiting for t2 to finish ..." << std::endl;
    t2.join();

    std::cout << "f2_called_counter = " << f2_called_counter << std::endl;

    std::cout << "End of threads example reached." << std::endl;
}