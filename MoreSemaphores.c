#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/mman.h>

// Globals
sem_t* semaphore;
pid_t otherPid;
sigset_t sigSet;

void signalHandler1(int signum) {
    printf("Caught Signal: %d\n", signum);
    printf("Exiting Child Process.\n");
    sem_post(semaphore);
    _exit(0);
}

void signalHandler2(int signum) {
    printf("I am alive!\n");
}

// Child process code
void childProcess() {
    // Signal Handlers
    signal(SIGUSR1, signalHandler1);
    signal(SIGUSR2, signalHandler2);
    
    int value;
    sem_getvalue(semaphore, &value);
    printf("Child Process Semaphore count is: %d\n", value);
    printf("Child Process is grabbing semaphore.\n");
    sem_wait(semaphore);
    sem_getvalue(semaphore, &value);
    printf("Child Process count is: %d.\n", value);
    
    printf("Starting long Child Process.\n");
    for(int i = 0; i < 60; ++i) {
        printf(".\n");
        sleep(1);
    }
    
    sem_post(semaphore);
    
    // Exiting Child Process
    printf("Exit Child Process.\n");
    _exit(0);
}

void *checkHungChild(void *a) {
    int* status = a;
    printf("Checking for hung Child Process...\n");
    sleep(10);
    
    if(sem_trywait(semaphore) != 0) {
        printf("Child Process is hung...\n");
        *status = 1;
    }
    else {
        printf("Child Process is running fine.\n");
        *status = 0;
    }
    return NULL;
}

void parentProcess() {
    // Detecting hung child and killing the process after set time
    sleep(2);
    if(getpgid(otherPid) >= 0) {
        printf("Child Process is running.\n");
    }
    
    int value;
    sem_getvalue(semaphore, &value);
    printf("In the Parent Process with a semaphore count of: %d.\n", value);
    
    // Try to get semaphore, otherwise start a timer
    if(sem_trywait(semaphore) != 0) {
        pthread_t tid1;
        int status = 0;
        
        printf("Detected Child Process that is hung...\n");
        if(pthread_create(&tid1, NULL, checkHungChild, &status)) {
            printf("Error creating timer thread.\n");
            _exit(1);
        }
        
        if(pthread_join(tid1, NULL)) {
            printf("Error joining timer thread.\n");
            exit(1);
        }
        
        // Determine if Child Process needs to be killed
        if(status == 1) {
            // Kill Child Process
            printf("Killing off child process with id of: %d\n", otherPid);
            kill(otherPid, SIGTERM);
            printf("Killed Child Process\n");
            
            printf("Proving that Child Process is killed.\n");
            sleep(5);
            kill(otherPid, SIGUSR2);
            sleep(1);
            printf("Done proving Child Process is killed.\n");
            
            // Try to get semaphore
            sem_getvalue(semaphore, &value);
            printf("In the Parent Process with semaphore count of: %d.\n", value);
            if(sem_trywait(semaphore) != 0) {
                if(value == 0) {
                    sem_post(semaphore);
                }
                
                printf("Cleaned up child process and got semaphore.\n");
                sem_getvalue(semaphore, &value);
                printf("In the Parent Process with semaphore count of: %d.\n", value);
            }
            
            else {
                printf("Finally got the semaphore.\n");
            }
        }
    }
    // Exit Parent Process
    printf("Exiting Parent Process. \n");
    _exit(0);
}

int main(int argc, char* argv[]) {
    pid_t pid;
    
    // Creating a shared semaphore
    semaphore = (sem_t*)mmap(0, sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    // Failing to create semaphore
    if(sem_init(semaphore, 1, 1) != 0){
        exit(EXIT_FAILURE);
    }
    
    // Forking processes
    pid = fork();
    if(pid == -1) {
        printf("Error forking.\n");
        exit(EXIT_FAILURE);
    }
    
    // Creating Parent and Child processes
    if(pid == 0) {
        printf("Started Child Process with Process ID: %d\n", getpid());
        otherPid = getpid();
        childProcess();
    }
    else {
        printf("Started Parent Process with Process ID: %d\n", getpid());
        otherPid = pid;
        parentProcess();
    }
    
    // Destroying semaphore
    sem_destroy(semaphore);
    
    return(0);
}
