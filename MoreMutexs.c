#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

pthread_mutex_t mutex;
int count;
time_t endTime;

void *counter(void *arg) {
    int status;
    
    while (time(NULL) < endTime) {
        status = pthread_mutex_lock(&mutex);
        // Locking mutex
        if(status == 0) {
            printf("Counter Thread: Locked Mutex.\n");
        }
        ++count;
        status = pthread_mutex_unlock(&mutex);
        // Unlocking mutex
        if(status == 0) {
            printf("Counter Thread: Unlocked Mutex\n");
        }
        sleep(1);
    }
    printf("Final Count: %d\n", count);
    return NULL;
}

void *monitor(void *arg) {
    int status;
    int misses = 0;
    
    while (time(NULL) < endTime) {
        sleep(3);
        
        // Try to gain access to the mutex and lock it, othewise count misses
        status = pthread_mutex_trylock(&mutex);
        
        if(status != EBUSY) {
            printf("Monitor Thread: Lock the mutex so we can read the counter.\n");
            printf("Monitor Thread: The Count from the Counter Thread is: %d\n", count);
            // Unlocking the mutex so the counter can gain access
            status = pthread_mutex_unlock(&mutex);
            if(status == 0) {
                printf("Monitor Thread: Unlocking the mutex.\n");
            }
        }
        else {
            // Counting misses if we can't lock the mutex
            ++misses;
        }
    }
    
    // Print the number of misses
    printf("Final number of Monitor Thread Misses: %d\n", misses);
    return NULL;
}

int main(int argc, char *argv[]) {
    int status;
    pthread_t counterThreadId;
    pthread_t monitorThreadId;
    
    // Initialize the mutex
    pthread_mutex_init(&mutex, 0);
    
    // Setting the time to 60 seconds
    endTime = time(NULL) + 60;
    
    // Creating the threads
    if(pthread_create(&counterThreadId, NULL, counter, NULL)) {
        printf("Error creating counter thread.\n");
    }
    if(pthread_create(&monitorThreadId, NULL, monitor, NULL)) {
        printf("Error creating monitor thread.\n");
    }
    
    // Telling the system to wait for the threads to finish
    if(pthread_join(counterThreadId, NULL)) {
        printf("Error joining counter thread.");
    }
    if(pthread_join(monitorThreadId, NULL)) {
        printf("Error joining monitor thread.");
    }
    
}
