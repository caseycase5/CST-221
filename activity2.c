/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


int t1, t2;
int balance = 0;
// Mutex Variable
pthread_mutex_t lock;


void *deposit(void* arg) {
    int *tNum = (int *)arg;
    
    // Mutex locking
    pthread_mutex_lock(&lock);
    
    printf("Thread %d has started \n", *tNum);
    
    // Iterating 1 Million Deposits
    for(int i = 0; i < 1000000; i++) {
        balance++;
    }
    printf("Balance after Thread: %d\n", balance);
    
    // Mutex unlocking
    pthread_mutex_unlock(&lock);
}

int main() {
    pthread_t thread1, thread2;
    t1 = 1;
    t2 = 2;
    
    // Creation of 2 Threads
    pthread_create(&thread1, NULL, deposit, (void *)&t1);
    pthread_create(&thread2, NULL, deposit, (void *)&t2);

    sleep(1);
    
    // Destroying the Mutex
    pthread_mutex_destroy(&lock);
    exit(0);
}
