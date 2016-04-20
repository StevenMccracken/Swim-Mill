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
pid_t fish, pellet;

void endProgram();
void catchFish(int signal) {}

int main() {
    // Setup signal to catch signals from fish
    sigset_t fish_mask;
    sigfillset(&fish_mask);
    sigdelset(&fish_mask, SIGUSR1);
    signal(SIGUSR1, catchFish);
    signal(SIGINT, endProgram);
    
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
                // Wait until fish sends signal that it has moved
                //sigsuspend(&fish_mask);
                
                // Print water stream
                printWater();
                sleep(1);
                printf("%d seconds passed\n", (seconds+1));
                
                // Alert fish process that water has been printed
                //kill(fish, SIGUSR2);
            }
            
            endProgram();
        }
    }
    return 0;
}

void endProgram() {
    kill(fish, SIGINT);
    kill(pellet, SIGINT);
    shmdt(water);
    printf("Swim_mill done\n");
    exit(0);
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
    int sharedMemoryID;
    
    // Create segment
    if((sharedMemoryID = shmget(key, sizeof(water), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    } else;
        //printf("Segment %d created\n", shmid);
    
    // Attach segment to data space
    if((water = shmat(sharedMemoryID, NULL, 0)) == (char *)-1) {
        perror("shmat");
        exit(1);
    } else;
        //printf("Attached segment to data space %p\n", water);
}