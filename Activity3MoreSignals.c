#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>

int main()
{

}

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

struct CIRCULAR_BUFFER *buffer = NULL;

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
    
    printf("Running child process...\n");
    
    // Reading all characters from buffer
    int character = 0;
    do {
        sleepAndWait();
        character = getValue();
    }while(character != 0);
    
    printf("Exiting child process.\n");
    _exit(1);
}

void producer() {
    
}





