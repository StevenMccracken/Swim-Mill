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

static void *child(int*);

void catch(int signal) {
    printf("got signal %d\n", signal);
}

int main() {
    srand(time(NULL));
    //signal(SIGUSR1, catch);
    attachSharedMemory();
    pthread_t threads[30];
    
    kill(getpid(), SIGSTOP);
    
    for(int i = 0; i < 30; i++) {
        // Sleeps for random interval between 1 and 2 seconds
        int sleepTime = rand() % 2 + 1;
        sleep(sleepTime);
        
        int xPos, yPos;
        do {
            xPos = rand()%8+1;
            yPos = rand()%8+1;
        }
        while((*water)[xPos][yPos] == 'o');
        
        int position[2] = {xPos, yPos};
        pthread_create(&threads[i], NULL, child, position);
    }
    
    shmdt(water);
    printf("Pellet done\n");
    exit(0);
}

static void *child(int *position) {
    bool eaten = false;
    int xpos = *position;
    int ypos = *(position+1);
    while(1) {
        if(ypos == rows)
            break;
        else if((*water)[ypos][xpos] == '^') {
            eaten = true;
            break;
        } else
            (*water)[ypos][xpos] = 'o';
        
        sleep(1);
        ypos++;
        
        if((*water)[ypos-1][xpos] != '^')
            (*water)[ypos-1][xpos] = '~';
    }
    printf("Pellet %d left stream at column %d\n", pthread_self(), (xpos+1));
    printf("Pellet %d eaten status: %d\n", pthread_self(), eaten);
}