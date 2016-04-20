//
//  includes.h
//  
//
//  Created by Steven McCracken on 4/14/16.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>

const key_t key = 1738;
const int rows = 10, cols = 10;
char (*water)[rows][cols];

void attachSharedMemory() {
    int sharedMemoryID;
    
    // Locate segment
    if((sharedMemoryID = shmget(key, sizeof(water), 0666)) < 0) {
        perror("shmget");
        exit(1);
    } else;
        //printf("Process %d located segment %d\n", getpid(), shmid);
    
    // Attach segment to data space
    if((water = shmat(sharedMemoryID, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    } else;
        //printf("Process %d attached segment to data space %p\n", getpid(), water);
}