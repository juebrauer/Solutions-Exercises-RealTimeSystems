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
    volatile double result = 0;

    printf("Press a key to start!\n\n");
    _getch();

    printf("Executing f1()...\n");
    result += f1();

    printf("Executing f2()...\n");
    result += f2();

    printf("Executing f3()...\n");
    result += f3();

    //printf("\nResult = %f\n", result);
    printf("Finished!\n");

    _getch();

} // end main 