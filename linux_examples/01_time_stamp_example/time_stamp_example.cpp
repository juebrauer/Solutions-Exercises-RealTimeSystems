// Simple example for showing
// how to measure code execution times
// using the time stamp method
//
// Note:
//
// First compile using
//    g++ time_stamp_example.cpp -o time_stamp_example.out (without optimizations)
//
// then using optimizations flag "O3", see:
//
//    https://www.linuxtopia.org/online_books/an_introduction_to_gcc/gccintro_49.html
//
//   g++ -O3 time_stamp_example.cpp -o time_stamp_example.out (with all optimizations turned on)
//   --> This will dramatically reduce execution time! Why? (then add "volatile" keyword before result)


#include <iostream> // for cout
#include <ctime>    // for clock()
#include <cmath>    // for cos()


double f1()
{
   double result = 0.0;
   for (int i = 0; i < 1e6; i++)
      result += cos(i);
   return result;
}

double f2()
{
   double result = 0.0;
   for (int i = 0; i < 1e7; i++)
      result += cos(i);
   return result;
}

double f3()
{
   double result = 0.0;
   for (int i = 0; i < 1e8; i++)
      result += cos(i);
   return result;
}


void show_duration_info(clock_t tic, clock_t toc)
{
   double time_elapsed = (double)(toc - tic) / CLOCKS_PER_SEC;
   std::cout << "tic=" << tic << 
                " toc=" << toc <<
                " --> time elpased=" << time_elapsed <<
                " seconds" << std::endl;
}


int main()
{

   clock_t tic, toc;
   double time_elapsed;
     
   double result = 0;

   std::cout << "CLOCKS_PER_SEC=" << CLOCKS_PER_SEC << std::endl;
   std::cout << "Press enter to continue!" << std::endl;
   std::cin.ignore();


   tic = clock();
   result += f1();   
   toc = clock();
   show_duration_info(tic,toc);
   

   tic = clock();
   result += f2();
   toc = clock();
   show_duration_info(tic,toc);


   tic = clock();
   result += f3();
   toc = clock();
   show_duration_info(tic,toc);

   //std::cout << "result=" << result << std::endl;
   
} // end main 