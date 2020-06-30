/// Threads - Producer / Consumer solution with mutex and
///           condition for 1 producer and 1 consumer
///
/// Note: Only the new C++ 20 standard (-std=c++20) will have
///       counting semaphores. For this, here we do not present
///       a solution for the producer-consumer problem with 3
///       semaphores, but a solution with a mutex and conditions,
///       which are both available with Posix threads.
///
/// See:
///  condition variables -
///  https://en.wikipedia.org/wiki/Monitor_(synchronization)#Condition_variables
///
/// This code is inspired by:
///  https://www.bogotobogo.com/cplusplus/multithreading_pthread.php
///
/// Note: for compiling with g++ type in:
///
///   g++ -pthread producer_consumer.cpp -o producer_consumer.out
///
/// What does the "-pthread" flag mean?
/// pthread
///    Adds support for multithreading with the pthreads library. This
///    option sets flags for both the preprocessor and linker.

///
/// ---
/// by Prof. Dr.-Ing. Juergen Brauer, www.juergenbrauer.org

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>  // for sleep() function
#include <stdlib.h>  // for rand() function

pthread_mutex_t mutex;
pthread_cond_t cond;

int buffer[100];

int loops = 5;
int length = 0;

void* producer(void *arg) {
    int i;
    for (i = 1; i <=loops; i++) {
        pthread_mutex_lock(&mutex);
        buffer[length++] = i;
        printf("Producing item: %d\n", i);        
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }
}

void* consumer(void *arg) {
    int i;
    for (i = 1; i <=loops; i++) {
        pthread_mutex_lock(&mutex);        
        while(length == 0) {
            printf(" consumer waiting...\n");
            pthread_cond_wait(&cond, &mutex);            
        }        
        int item = buffer[--length];
        printf("Consuming item: %d\n", item);
        pthread_mutex_unlock(&mutex);

        sleep(1 + rand() % 5);
    }
}


int main(int argc, char *argv[])
{
    pthread_mutex_init(&mutex, 0);
    pthread_cond_init(&cond, 0);

    pthread_t pThread, cThread;
    pthread_create(&pThread, 0, producer, 0);
    pthread_create(&cThread, 0, consumer, 0);
    pthread_join(pThread, NULL);
    pthread_join(cThread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}