#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

void* create_shared_memory(size_t size) {
    int protection = PROT_READ | PROT_WRITE;
    
    int visibility = MAP_SHARED | MAP_ANONYMOUS;
    
    return mmap(NULL, size, protection, visibility, -1, 0);
}


void main() {
    char parent_message[] = "Written by Parent";
    char child_message[] = "Written by Child";
    
    void* shmem = create_shared_memory(128);
    
    int pid = fork();
    
    if(pid == 0) {
        printf("Child read: %s\n", shmem);
        memcpy(shmem, child_message, sizeof(child_message));
        printf("Child wrote: %s\n", shmem);
    }
    else {
        printf("Parent read: %s\n", shmem);
        sleep(1);
        printf("After 1s, Parent read: %s\n", shmem);
        memcpy(shmem, parent_message, sizeof(parent_message));
        printf("Parent read: %s\n", shmem);
    }
}
