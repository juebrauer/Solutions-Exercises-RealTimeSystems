#include <stdio.h> 
#include <conio.h> 
#include <math.h>
#include <time.h> 


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


int main()
{

   clock_t tic, toc;
     
   volatile double result = 0;

   printf("Press a key to start!\n\n");
   _getch();

   tic = clock();
   result += f1();   
   toc = clock();
   printf("tic=%d, toc=%d --> Time elapsed for f1(): %f seconds\n",
       tic, toc, (double)(toc - tic) / CLOCKS_PER_SEC);

   tic = clock();
   result += f2();
   toc = clock();
   printf("tic=%d, toc=%d --> Time elapsed for f2(): %f seconds\n",
       tic, toc, (double)(toc - tic) / CLOCKS_PER_SEC);

   tic = clock();
   result += f3();
   toc = clock();
   printf("tic=%d, toc=%d --> Time elapsed for f3(): %f seconds\n",
       tic, toc, (double)(toc - tic) / CLOCKS_PER_SEC);

   //printf("\nResult = %f\n", result);
      
   _getwch();

} // end main 