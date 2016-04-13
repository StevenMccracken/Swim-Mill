//
//  swim_mill.c
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
#include <sys/wait.h>

#define SHMSZ 100

int main() {
    pid_t fish, pellet;
    int shmid;
    key_t key = 1738;
    char *shm;
    char (*water)[10][10];
    
    char c = 0x80;
    printf("%c\n", c);
    // Create segment
    if((shmid = shmget(key, sizeof(water), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    } else
        printf("Segment %d created\n", shmid);
    
    // Attach segment to data space
    if((water = shmat(shmid,NULL,0)) == (char *)-1) {
        perror("shmat");
        exit(1);
    } else
        printf("Attached segment to data space %p\n", water);
    
    // Put stuff into memory
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            (*water)[i][j] = '~';
        }
    }
    (*water)[9][4] = '^';
    
    
    if((fish = fork()) == 0) {
        if((pellet = fork()) == 0) {
            int seconds = 0;
            while(seconds < 30) {
                for(int i = 0; i < 10; i++) {
                    for(int j = 0; j < 10; j++) {
                        printf("%c",(*water)[i][j]);
                    }
                    printf("\n");
                }
                printf("\n");
                sleep(1);
                seconds++;
            }
        } else { // pellet
            sleep(1);
            static char *argv[] = {"","",NULL};
            execv("/Users/stevenmccracken/Documents/GitHub/School/Swim-Mill/pellet", argv);
        }
    } else { // fish
        static char *argv[] = {"","",NULL};
        execv("/Users/stevenmccracken/Documents/GitHub/School/Swim-Mill/fish", argv);
    }
    return 0;
}