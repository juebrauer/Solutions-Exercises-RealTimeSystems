#include <iostream> // for cout
#include <cmath>    // for cos()

#include "/home/juebrauer/link_to_vcd/tmp/gperftools-master/src/gperftools/profiler.h" // for speed profiling


double f1()
{
   double result = 0.0;
   for (int i = 0; i < 1e3; i++)
      result += cos(i);
   return result;
}

double f2()
{
   double result = 0.0;
   for (int i = 0; i < 1e4; i++)
      result += cos(i);
   return result;
}

double f3()
{
   double result = 0.0;
   for (int i = 0; i < 1e5; i++)
      result += cos(i);
   return result;
}


double f4_1()
{
    return 3.14159;
}

double f4_2()
{
    return exp(3.14159);
}

double f4()
{
    double x;
    x = f4_1() + f4_2();
    return x;
}



int main()
{
   double result = 0;

   // ProfilerStart("profile.log");

   for (int i=1; i<=5; i++)
   {
     result += f1();   
     result += f2();
     result += f3();
     result += f4(); 
   }

   // ProfilerStop();

   std::cout << "result=" << result << std::endl;
}