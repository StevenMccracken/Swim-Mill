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

#define SHMSZ   27

int main() {
    pid_t fish, pellet;
    int shmid;
    key_t key = 1738;
    char *shm;
    
    // Create segment
    if((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    } else
        printf("Segment %d created\n", shmid);
    
    // Attach segment to data space
    if((shm = shmat(shmid,NULL,0)) == (char *)-1) {
        perror("shmat");
        exit(1);
    } else
        printf("Attached segment to data space %p\n", shm);
    
    *shm = 'q';
    printf("hey\n");
    
    if((fish = fork()) == 0) {
        if((pellet = fork()) == 0) {
            //printf("%p: %c\n", shm, *shm);
            printf("Parent started\n");
            printf("Parent done\n");
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