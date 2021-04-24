#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>

// Buffer
int MAX = 100;
int WAKEUP = SIGUSR1;
int SLEEP = SIGUSR2;

pid_t otherPid;

sigset_t sigSet;

struct CIRCULAR_BUFFER {
    int count;
    int lower;
    int upper;
    int buffer[100];
}

struct CIRCULAR_BUFFER* buffer = NULL;

// Sleep Method
void sleepAndWait() {
    int nSig;
    
    // Sleep until wake up signal received
    printf("Sleeping..\n");
    sigwait(&sigSet, &nSig);
    printf("Awaken Signal\n");
}

// Wake up Method
void wakeUp() {
    kill(otherPid, WAKEUP);
}

// Gets value from buffer
int getValue() {
    int value = buffer->buffer[buffer->lower];
    printf("Consumer read value of %c\n", value);
    ++buffer->lower;
    // Returning buffer to beginning when it reaches the end
    if(buffer->lower == MAX) {
        buffer->lower = 0;
    }
    --buffer->count;
    return value;
}

// Puts a value in the buffer
void putValue(int value) {
    // Writes value to buffer
    buffer->buffer[buffer->upper] = value;
    printf("Producer stored value of %c\n", buffer->buffer[buffer->upper]);
    ++buffer->upper;
    // Resets buffer to start if end is reached
    if(buffer->upper == MAX) {
        buffer->upper = 0;
    }
    ++buffer->count;
}

void consumer() {
    // Setup signals
    sigemptyset(&sigSet);
    sigaddset(&sigSet, WAKEUP);
    sigprocmask(SIG_BLOCK, &sigSet, NULL);
    
    printf("Running Child Process...\n");
    
    // Reading all characters from buffer
    int character = 0;
    do {
        sleepAndWait();
        character = getValue();
    }
    while(character != 0);
    
    printf("Exiting Child Process.\n");
    _exit(1);
}

void producer() {
    int value = 0;
    printf("Running the Parent Process...\n");
    
    char message[10] = "Casey Huz";
    for(int i = 0; i < strlen(message); i++) {
        // Puts the values of the string into the buffer
        putValue(message[i]);
        wakeUp();
        sleep(1);
    }
    // Tells child process that the parent has completed
    putValue(0);
    wakeUp();
    
    printf("Exiting the Parent Process.\n");
    _exit(1);
}

int main() {
    pid_t pid;
    
    // Create the Buffer
    buffer = (struct CIRCULAR_BUFFER*)mmap(0, 100, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    buffer->count = 0;
    buffer->lower = 0;
    buffer->upper = 0;
    
    pid = fork();
    
    // Printing an error if PID = -1
    if(pid == -1) {
        printf("Error forking.");
        exit(EXIT_FAILURE);
    }
    
    // Executing Child Process
    if(pid==0) {
        otherPid = pid;
        producer();
    }
    // Executing Parent Process
    else {
        otherPid = pid;
        consumer();
    }
    return 0;
}
