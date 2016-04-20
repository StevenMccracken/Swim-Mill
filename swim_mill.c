//
//  swim_mill.c
//  
//
//  Created by Steven McCracken on 4/10/16.
//
//

#include "includes.h"

void catchKillSig();
void endProgram();
void printWater();
void configureWater();
void createSharedMemory();
void writeResults();

const int timeLimit = 30;
pid_t fish, pellet;

int main() {
    printf("PID %d (swim mill) started\n", getpid());
    
    // Setup signal intercept for ^C
    signal(SIGINT, catchKillSig);
    
    // Create water in shared memory
    createSharedMemory();
    configureWater();
    
    // Start child processes
    if((fish = fork()) == 0) {
        static char *argv[] = {"","",NULL};
        execv("/Users/stevenmccracken/Documents/GitHub/School/Swim-Mill/fish", argv);
    } else {
        if((pellet = fork()) == 0) {
            static char *argv[] = {"","",NULL};
            execv("/Users/stevenmccracken/Documents/GitHub/School/Swim-Mill/pellet", argv);
        } else {
            // Run fish and pellet processes for timeLimit seconds
            for(int seconds = 0; seconds < timeLimit; seconds++) {
                sleep(1);
                printWater();
                //printf("%d seconds passed\n", (seconds+1));
            }
            endProgram();
        }
    }
    return 0;
}

void catchKillSig() {
    // Save results from swim mill
    writeResults();
    
    // Kill child processes because user entered ^C
    printf("Swim mill received ^C signal\n");
    kill(fish, SIGINT);
    kill(pellet, SIGINT);
    
    // Deallocate shared memory
    shmdt(water);
    
    printf("PID %d (swim mill) killed because of ^C\n", getpid());
    exit(0);
}

void endProgram() {
    // Save results from swim mill
    writeResults();
    
    // Kill child processes because time limit has expired
    printf("%d second time limit expired!\n", timeLimit);
    kill(fish, SIGUSR1);
    kill(pellet, SIGUSR1);
    
    // Deallocate shared memory
    shmdt(water);
    
    printf("PID %d (swim mill) exited because time limit expired\n", getpid());
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

void writeResults() {
    FILE *fp;
    fp = fopen("/Users/stevenmccracken/Desktop/results.txt", "w+");
    if(fp == NULL) {
        printf("PID %d (swim mill) exited because of failure to write results\n", getpid());
        exit(1);
    } else {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                char waterChar = (*water)[i][j];
                fprintf(fp, "%c", waterChar);
            }
            fprintf(fp, "\n");
        }
        fclose(fp);
    }
}