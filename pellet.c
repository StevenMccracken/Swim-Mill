//
//  pellet.c
//  
//
//  Created by Steven McCracken on 4/10/16.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#define SHMSZ   27

int main() {
    srand(time(NULL));
    int shmid;
    key_t key = 1738;
    char (*water)[10][10];
    
    // Locate segment
    if((shmid = shmget(key, sizeof(water), 0666)) < 0) {
        perror("shmget");
        exit(1);
    } else {
        //printf("Pellet located segment %d\n", shmid);
    }
    
    // Attach segment to data space
    if((water = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    } else {
        //printf("Pellet attached segment to data space %p\n", water);
    }
    
    int xpos, ypos;
    xpos = rand() % 10;
    ypos = rand() % 10;
    while(1) {
        (*water)[ypos][xpos] = 'o';
        sleep(1);
        ypos++;
        (*water)[ypos-1][xpos] = '~';
        if(ypos == 10) break;
    }
    
    printf("Pellet done\n");
    exit(0);
}