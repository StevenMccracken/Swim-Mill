//
//  Test.c
//  
//
//  Created by Steven McCracken on 4/9/16.
//
//

#include "Test.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHMSZ   27

main() {
    char c;
    int shmid;
    key_t key;
    char *shm, *s;
    
    key = 1738;
    
    // Create segment
    if((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    } else {
        printf("ayy1");
    }
    
    // Attach segment to data space
    if((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    } else {
        printf("ayy2");
    }
    
    // Put things into the memory
    s = shm;
    
    for(c = 'a'; c <= 'z'; c++)
        *s++ = c;
    *s = NULL;
    
    printf("ayy3");
}