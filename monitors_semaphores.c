
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

int canChat = 1;
int u1, u2, u3, u4, u5;

// Semaphore Variable
sem_t mutex;

void *chat(void* arg) {
    int *userNum = (int *)arg;
    
    // Mutex locking
    //pthread_mutex_lock(&lock);
    
    // Semaphore locking
    sem_wait(&mutex);
    
    // Actions during thread
    printf("User %d has joined the chat room.\n", *userNum);
    printf("User %d is chatting...\n", *userNum);\
    if(canChat == 1) {
        for(int i = 0; i < 5; i++) {
            printf("User %d Message Sent.\n", *userNum);
            sleep(5);
        }
    }
    printf("User %d has left the chat room.\n", *userNum);
   
    // Mutex unlocking
    //pthread_mutex_unlock(&lock);
    
    // Semaphore unlocking
    sem_post(&mutex);
}

int main() {
    pthread_t user1, user2, user3, user4, user5;
    u1 = 1;
    u2 = 2;
    u3 = 3;
    u4 = 4;
    u5 = 5;
    
    // Initializing the Semaphore
    sem_init(&mutex, 0, 1);
    
    // Creation of 5 Threads
    pthread_create(&user1, NULL, chat, (void *)&u1);
    pthread_create(&user2, NULL, chat, (void *)&u2);
    pthread_create(&user3, NULL, chat, (void *)&u3);
    pthread_create(&user4, NULL, chat, (void *)&u4);
    pthread_create(&user5, NULL, chat, (void *)&u5);

    pthread_join(user1, NULL);
    pthread_join(user2, NULL);
    pthread_join(user3, NULL);
    pthread_join(user4, NULL);
    pthread_join(user5, NULL);
    
    sem_destroy(&mutex);
    exit(0);
}
