#include <stdio.h>  // for printf()
#include <conio.h>  // for _getch()
#include <stdlib.h> // for rand()
#include <time.h>   // for clock()

#define ARRAY_SIZE 10000000
#define NR_EXPS    10000000

int loop_counter;
int found_counter;

///
/// Binary search algorithm
/// reference code: https://de.wikipedia.org/wiki/Bin%C3%A4re_Suche#C
///

int binary_search(const int A[], const int N, const int search_value)
{
   int middle;
   int left = 0;
   int right = N - 1;
   
   while (left <= right)
   {
      loop_counter++;
      middle = (left+right)/2;

      if (A[middle] == search_value)
      {
         break;
      }
      else
         if (A[middle] > search_value)
            right = middle - 1;     // continue searching to the left
         else
            left = middle + 1;      // continue searching to the right                                    
   }

   bool found = (A[middle] == search_value);

   //printf("\nLoops needed: %d\n", loop_counter);
   if (found)
   {
       found_counter++;
      //printf("Found the number %d in the array at location %d!\n", search_value, middle);
      return middle;
   }
   else
   {
      //printf("Could not find the number %d in the array!\n", search_value);
      return -1;
   }
   
} // binary_search



int* setup_strictly_monotonous_increasing_random_array(int size)
{
   int* A = new int[size];

   A[0] = 0;
   for (int i=1; i<size; i++)
      A[i] = A[i-1] + 5;

   printf("\nValue of first element (nr. 0) in array is : %d", A[0]);
   printf("\nValue of last element (nr. %d) in array is : %d\n", size-1, A[size-1]);

   return A;

} // setup_strictly_monotonous_increasing_random_array


int main()
{
   clock_t tic, toc;
   double computation_time;

   printf("Exercise: WCET analysis\n");
   printf("-------------------------\n\n");

   printf("\t Search array size            : %d\n", ARRAY_SIZE);
   printf("\t Number of search experiments : %d\n", NR_EXPS);

   // 1. prepare some array
   int* A = setup_strictly_monotonous_increasing_random_array( ARRAY_SIZE );

    
   // 2. estimate average case runtime    
   printf("\n\nAverage Case Execution Runtime test:\n");
   loop_counter = 0;
   found_counter = 0;
   tic = clock(); // returns the processor time consumed by the program
   for (int expnr=0; expnr<NR_EXPS; expnr++)
   {
      binary_search(A, ARRAY_SIZE, A[expnr]);
   }
   toc = clock();
   computation_time = (double)(toc - tic) / CLOCKS_PER_SEC;
   printf("Results:\n");
   printf("\t Total number of loops done        : %d\n", loop_counter);
   printf("\t Number found                      : %d times\n", found_counter);
   printf("\t Total runtime for all experiments : %.15f sec\n", computation_time);   
   printf("\t Estimated average case runtime    : %.15f sec\n", computation_time / (double)NR_EXPS);


   // 3. estimate WCET
   int value_that_is_not_in_array;
   value_that_is_not_in_array = A[ARRAY_SIZE - 1] + 1;
   //value_that_is_not_in_array = -1;
   printf("\n\nWorst Case Execution Runtime (WCET) test:\n");
   printf("\nSearching for number %d in array\n", value_that_is_not_in_array);
   loop_counter = 0;
   found_counter = 0;
   tic = clock();
   for (int expnr = 0; expnr<NR_EXPS; expnr++)
   {
      binary_search(A, ARRAY_SIZE, A[expnr]+1);
      //binary_search(A, ARRAY_SIZE, value_that_is_not_in_array);
   }
   toc = clock();
   computation_time = (double)(toc - tic) / CLOCKS_PER_SEC;
   printf("Results:\n");
   printf("\t Total number of loops done        : %d\n", loop_counter);
   printf("\t Number found                      : %d times\n", found_counter);
   printf("\t Total runtime for all experiments : %.15f sec\n", computation_time);
   printf("\t Estimated WCET                    : %.15f sec\n", computation_time / (double)NR_EXPS);
   
   
   printf("\nFinished. Press a key to exit.\n");
   _getch();
}