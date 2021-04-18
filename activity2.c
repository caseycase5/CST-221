#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>


int t1, t2;
int balance = 0;

// Mutex Variable
pthread_mutex_t lock;

// Semaphore Variable
sem_t mutex;

void *deposit(void* arg) {
    int *tNum = (int *)arg;
    
    // Mutex locking
    //pthread_mutex_lock(&lock);
    
    // Semaphore locking
    sem_wait(&mutex);
    
    printf("Thread %d has started \n", *tNum);
    
    // Iterating 1 Million Deposits
    for(int i = 0; i < 1000000; i++) {
        balance++;
    }
    printf("Balance after Thread: %d\n", balance);
    
    // Mutex unlocking
    //pthread_mutex_unlock(&lock);
    
    // Semaphore unlocking
    sem_post(&mutex);
}

int main() {
    pthread_t thread1, thread2;
    t1 = 1;
    t2 = 2;
    
    // Initializing the Semaphore
    sem_init(&mutex, 0, 1);
    
    // Creation of 2 Threads
    pthread_create(&thread1, NULL, deposit, (void *)&t1);
    pthread_create(&thread2, NULL, deposit, (void *)&t2);

    sleep(1);
    
    // Destroying the Mutex
    //pthread_mutex_destroy(&lock);
    sem_destroy(&mutex);
    exit(0);
}
