// from http://en.cppreference.com/w/cpp/chrono/c/clock
//
// "This example demonstrates the difference between clock() time and real time"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <conio.h>

int main()
{
    std::cout << "Press return to end the program!" << std::endl;

    std::clock_t c_start = std::clock();    
    auto t_start = std::chrono::high_resolution_clock::now();

    getchar();

    std::clock_t c_end = std::clock();
    auto t_end = std::chrono::high_resolution_clock::now();

    std::cout << "Thank you for pressing return!" << std::endl;

    std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
        << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n"
        << "Wall clock time passed: "
        << std::chrono::duration<double, std::milli>(t_end - t_start).count()
        << " ms\n";

    _getch();
}