#include <random> 
#include <stdio.h> 
#include <conio.h> 
#include <time.h> 

#define N 10000 
int numbers[N];

volatile float further_computations = 0.0;

void generate_random_numbers() {
    for (int i = 0; i < N; i++)
        numbers[i] = rand() % N;
}

void generate_worst_case_order() {
    for (int i = 0; i < N; i++)
        numbers[i] = N-i;
}

void S() {
    int counter_swaps = 0;
    bool ready;
    do {
        ready = true;
        for (int i = 0; i < N - 1; i++) {
            if (numbers[i] > numbers[i + 1]) {
                counter_swaps++;
                int tmp = numbers[i];
                numbers[i] = numbers[i + 1];
                numbers[i + 1] = tmp;   
                ready = false;
            }                            
        }
    } while (!ready);
    printf("counter_swaps = %d\n", counter_swaps);
}

int main() {
    srand((unsigned int) time(NULL));
    
    char c;
    do {
        generate_random_numbers();
        //generate_worst_case_order();
        clock_t tic, toc;
        tic = clock(); // returns the processor time consumed by the program
        S();
        toc = clock();
        double computation_time = (double)(toc - tic) / CLOCKS_PER_SEC;
        printf("Time needed: %.3f sec\n", computation_time);
        c = _getwch();
    } while (c!=29);
}
