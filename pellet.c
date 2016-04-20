//
//  pellet.c
//  
//
//  Created by Steven McCracken on 4/10/16.
//
//

#include "includes.h"
#include <time.h>
#include <pthread.h>

void endPellet();
static void *child(int*);

static int maxThreadsAlive = 20, totalThreads = 100;

int main() {
    // Setup catch for termination from swim_mill
    signal(SIGINT, endPellet);
    
    // Attach process to shared memory
    attachSharedMemory();
    
    srand(time(NULL)); // Generate random seed
    pthread_t threads[totalThreads]; // Allocate pellet threads
    
    printf("Pellet process %d started\n", getpid());
    
    for(int i = 0; i < totalThreads; i++) {
        // Sleep for random interval between 1 and 2 seconds
        int sleepTime = rand() % 2 + 1;
        sleep(sleepTime);
        
        // Generate random position in stream where another pellet doesn't currently exist
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
    endPellet();
}

void endPellet() {
    shmdt(water);
    printf("Pellet process %d done\n", getpid());
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
        
        // Overwrite previous position with water
        if((*water)[ypos-1][xpos] != '^')
            (*water)[ypos-1][xpos] = '~';
        
        if(eaten || leftStream)
            break;
    }
    
    if(eaten)
        printf("Pellet %d was eaten at column %d!\n", pthread_self(), xpos);
    else
        printf("Pellet %d wasn't eaten and left stream at column %d\n", pthread_self(), xpos);
}