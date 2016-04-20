#include "includes.h"
#include <time.h>
#include <pthread.h>

void catchKillSig();
void catchEndSig();
static void *child(int*);

static int maxThreadsAlive = 20, totalThreads = 100;

int main() {
    printf("PID %d (pellet) started\n", getpid());
    
    // Setup signal intercepts from swim_mill
    signal(SIGINT, catchKillSig);
    signal(SIGUSR1, catchEndSig);
    
    // Attach process to shared memory
    attachSharedMemory();
    
    srand(time(NULL)); // Generate random seed
    pthread_t threads[totalThreads]; // Allocate pellet threads
    
    for(int i = 0; i < totalThreads; i++) {
        // Sleep for random interval between 1 and 2 seconds
        int sleepTime = rand() % 2 + 1;
        sleep(sleepTime);
        
        /* Generate random position in stream where
         another pellet doesn't currently exist */
        int xPos, yPos;
        do {
            xPos = rand()%8+1;
            yPos = rand()%8+1;
        }
        while((*water)[xPos][yPos] == 'o');
        
        // Do nothing while maxThreadsAlive threads exist
        int aliveThreads;
        do {
            aliveThreads = 0;
            for(int j = 0; j < totalThreads; j++) {
                if(pthread_kill(threads[j], 0) == 0)
                    aliveThreads++;
            }
        } while(aliveThreads > maxThreadsAlive);
        
        // Spawn new pellet thread
        int position[2] = {xPos, yPos};
        pthread_create(&threads[i], NULL, child, position);
    }
    pthread_join(threads[totalThreads-1], NULL);
    
    shmdt(water);
    printf("PID %d (pellet) exited because last thread died\n", getpid());
    exit(0);
}

void catchKillSig() {
    shmdt(water);
    printf("PID %d (pellet) killed because of ^C\n", getpid());
    exit(0);
}

void catchEndSig() {
    shmdt(water);
    printf("PID %d (pellet) killed because time limit reached\n", getpid());
    exit(0);
}

static void *child(int *position) {
    // Obtain positions from args passed in when child thread starts
    int xpos = *position;
    int ypos = *(position+1);
    bool eaten = false, leftStream = false;
    
    printf("Pellet %d was dropped at [%d,%d]\n", pthread_self(), ypos, xpos);
    while(1) {
        (*water)[ypos][xpos] = 'o';
        
        // Advance pellet downwards in stream
        sleep(1);
        ypos++;
        
        // Check if pellet was eaten or left stream
        if(ypos == rows)
            leftStream = true;
        else if((*water)[ypos][xpos] == '^')
            eaten = true;
        
        // Overwrite previous position with water symbol
        if((*water)[ypos-1][xpos] != '^')
            (*water)[ypos-1][xpos] = '~';
        
        if(eaten || leftStream)
            break;
    }
    
    // Print pellet results to screen
    if(eaten)
        printf("Pellet %d was eaten at column %d!\n", pthread_self(), xpos);
    else
        printf("Pellet %d wasn't eaten and left stream at column %d\n", pthread_self(), xpos);
    
    // Append pellet results to file
    FILE *fp;
    fp = fopen("/Users/stevenmccracken/Desktop/results.txt", "a");
    if(fp == NULL) {
        printf("TID %d (pellet) failed to write results\n", pthread_self());
        exit(1);
    } else {
        if(eaten)
            fprintf(fp, "Pellet %d was eaten at column %d!\n", pthread_self(), xpos);
        else
            fprintf(fp, "Pellet %d wasn't eaten and left stream at column %d\n", pthread_self(), xpos);
        fclose(fp);
    }
}