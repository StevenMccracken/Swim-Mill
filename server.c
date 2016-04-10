//
//  server.c
//  
//
//  Created by Steven McCracken on 4/10/16.
//
//

#include "server.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSZ   27

main() {
    int shmid;
    key_t key;
    char *shm, *s;
    
    // Get segment created by server
    key = 1738;
    
    // Locate segment
    if((shmid = shmget(key, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    } else {
        printf("ayy4");
    }
    
    // Attach segment to data space
    if((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    } else {
        printf("ayy5");
    }
    
    // Read what server put in memory
    for(s = shm; *s != NULL; s++)
        putchar(*s);
    putchar('\n');
    
    // Change first character in segment
    *shm = '*';
    
    printf("Server done");
    exit(0);
}