//
//  fish.c
//  
//
//  Created by Steven McCracken on 4/9/16.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSZ   27

int main() {
    int shmid;
    key_t key = 1738;
    char *shm;
    
    // Locate segment
    if((shmid = shmget(key, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    } else {
        printf("Fish located segment %d\n", shmid);
    }
    
    // Attach segment to data space
    if((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    } else {
        printf("Fish attached segment to data space %p\n", shm);
    }
    
    // Wait until other process changes first char to *
    while(*shm == 'q') {
        printf("Fish reads: %c\n", *shm);
        sleep(1);
    }
    printf("Fish done\n");
    exit(0);
}