//
//  swim_mill.c
//  
//
//  Created by Steven McCracken on 4/10/16.
//
//

#include "includes.h"

void printWater();
void configureWater();
void createSharedMemory();
const int timeLimit = 30;

void catchFish(int signal) {
    
}

void catchPellet(int signal) {
    
}

int main() {
    sigset_t fish_mask, pellet_mask;
    sigfillset(&fish_mask);
    sigfillset(&pellet_mask);
    
    sigdelset(&fish_mask, SIGUSR1);
    sigdelset(&pellet_mask, SIGUSR2);
    
    signal(SIGUSR1, catchFish);
    signal(SIGUSR2, catchPellet);
    
    pid_t fish, pellet;
    
    createSharedMemory();
    configureWater();
    
    if((fish = fork()) == 0) {
        static char *argv[] = {"","",NULL};
        execv("/Users/stevenmccracken/Documents/GitHub/School/Swim-Mill/fish", argv);
    } else {
        if((pellet = fork()) == 0) {
            static char *argv[] = {"","",NULL};
            execv("/Users/stevenmccracken/Documents/GitHub/School/Swim-Mill/pellet", argv);
        } else {
            printf("Swim mill started\n");
            // Run fish and pellet processes for timeLimit seconds
            for(int seconds = 0; seconds < timeLimit; seconds++) {
                sigsuspend(&fish_mask);
                
                printWater();
                sleep(1);
                printf("%d seconds passed\n", (seconds+1));
                kill(fish, SIGUSR2);
                //kill(pellet, SIGUSR2);
            }
            
            kill(fish, SIGKILL);
            kill(pellet, SIGKILL);
            shmdt(water);
            printf("Swim_mill done\n");
        }
    }
    return 0;
}

void printWater() {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++)
            printf("%c",(*water)[i][j]);
        printf("\n");
    }
    printf("\n");
}

void configureWater() {
    // Place "water" into memory
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++)
            (*water)[i][j] = '~';
    }
    // Place fish in middle of last row
    (*water)[rows-1][cols/2] = '^';
}

void createSharedMemory() {
    int shmid;
    key_t key = 1738;
    char *shm;
    
    // Create segment
    if((shmid = shmget(key, sizeof(water), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    } else;
        //printf("Segment %d created\n", shmid);
    
    // Attach segment to data space
    if((water = shmat(shmid,NULL,0)) == (char *)-1) {
        perror("shmat");
        exit(1);
    } else;
        //printf("Attached segment to data space %p\n", water);
}