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

int main() {
    pid_t fish, pellet;
    
    createSharedMemory();
    configureWater();
    
    if((fish = fork()) == 0) {
        if((pellet = fork()) == 0) {
            
            // Run fish and pellet processes for timeLimit seconds
            for(int seconds = 0; seconds < timeLimit; seconds++) {
                printWater();
                sleep(1);
                //printf("%d seconds passed\n", (seconds+1));
            }
            kill(pellet, SIGUSR1);
            
            //kill(pellet, SIGKILL);
            kill(fish, SIGKILL);
            shmdt(water);
        } else { // pellet
            sleep(1);
            static char *argv[] = {"","",NULL};
            execv("/Users/stevenmccracken/Documents/GitHub/School/Swim-Mill/pellet", argv);
        }
    } else { // fish
        static char *argv[] = {"","",NULL};
        execv("/Users/stevenmccracken/Documents/GitHub/School/Swim-Mill/fish", argv);
    }
    
    printf("Swim_mill done\n");
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
    } else
        printf("Segment %d created\n", shmid);
    
    // Attach segment to data space
    if((water = shmat(shmid,NULL,0)) == (char *)-1) {
        perror("shmat");
        exit(1);
    } else
        printf("Attached segment to data space %p\n", water);
}