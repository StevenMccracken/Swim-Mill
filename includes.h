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

const int rows = 10, cols = 10;
char (*water)[rows][cols];

void attachSharedMemory() {
    int shmid;
    key_t key = 1738;
    
    // Locate segment
    if((shmid = shmget(key, sizeof(water), 0666)) < 0) {
        perror("shmget");
        exit(1);
    } else {
        //printf("Fish located segment %d\n", shmid);
    }
    
    // Attach segment to data space
    if((water = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    } else {
        //printf("Fish attached segment to data space %p\n", water);
    }
}