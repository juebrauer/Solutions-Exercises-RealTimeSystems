#include <random>   // for rand()
#include <iostream> // for cout

#include <time.h> 


#define N 20000 
int numbers[N];


void generate_random_numbers() {
    for (int i = 0; i < N; i++)
        numbers[i] = rand() % N;
}

void generate_worst_case_order() {
    for (int i = 0; i < N; i++)
        numbers[i] = N-i;
        // numbers[0] = 10000 = N
        // numbers[1] = 9999  = N-1
        // numbers[2] = 9998  = N-2
        // ...
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
    std::cout << "counter_swaps = " << counter_swaps << std::endl;
}

int main() {
    srand((unsigned int) time(NULL));
    
    double WCET = 0.0;

    for (int test_run=1; test_run<=5; test_run++)
    {
        std::cout << "\nTest run #" << test_run << std::endl;

        // 1. generate an array with numbers to be sorted
        //generate_random_numbers();
        generate_worst_case_order();

        // 2. sort the array and
        //    use the time stamp method in order to
        //    measure the CPU time needed to sort the array
        clock_t tic, toc;
        tic = clock(); // returns the processor time consumed by the program
        S();
        toc = clock();

        // 3. show CPU time needed to sort the array
        double computation_time = (double)(toc - tic) / CLOCKS_PER_SEC;
        std::cout << "Time needed: " << computation_time << "sec" << std::endl;

        if (computation_time > WCET)
            WCET = computation_time;
    }

    std::cout << "\n---\n";
    std::cout << "WCET: " << WCET << "sec\n";
    std::cout << "\nProgram end.\n";
}
