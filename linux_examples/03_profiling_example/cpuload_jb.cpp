#include <iostream> // for cout
#include <cmath>    // for cos()

//include "/home/juebrauer/link_to_vcd/tmp/gperftools-master/src/gperftools/profiler.h" // for speed profiling

#define HOW_OFTEN 1e7


double f1()
{
   double result = 0.0;
   for (int i = 0; i < HOW_OFTEN; i++)
      result += cos(i);
   return result;
}

double f2()
{
   double result = 0.0;
   for (int i = 0; i < HOW_OFTEN; i++)
      result += cos(i);
   return result;
}

double f3()
{
   double result = 0.0;
   for (int i = 0; i < HOW_OFTEN; i++)
      result += cos(i);
   return result;
}


double b()
{
    return 3.14159;
}

double a()
{
    return b();
}

double f4()
{    
   double result = 0.0;
   for (int i = 0; i < HOW_OFTEN; i++)
      result += a();
   return result;
}



int main()
{
   double result = 0;

   // ProfilerStart("profile.log");

   for (int i=1; i<=10; i++)
   {
     result += f1();   
     result += f2();
     result += f3();
     result += f4(); 
   }

   // ProfilerStop();

   std::cout << "result=" << result << std::endl;
}