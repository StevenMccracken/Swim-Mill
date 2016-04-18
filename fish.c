//
//  fish.c
//  
//
//  Created by Steven McCracken on 4/9/16.
//
//

#include "includes.h"

int scanRow(int, int);
int currentLocation = rows/2;

void catch(int signal) {
    
}

int main() {
    sigset_t mill_mask;
    sigfillset(&mill_mask);
    sigdelset(&mill_mask, SIGUSR2);
    signal(SIGUSR2, catch);
    
    attachSharedMemory();
    //sigsuspend(&mill_mask);
    printf("Fish started\n");
    
    while(1) {
        printf("Checking for pellets\n");
        // Locate
        int pelletLocations[rows];
        for(int i = 0; i < rows; i++) {
            pelletLocations[i] = scanRow(i, currentLocation);
        }
        for(int i = 0; i < rows; i++) {
            if(pelletLocations[i] != -1) {
                int xDistance = currentLocation - pelletLocations[i];
                if(xDistance != 0) {
                    int move = 1;
                    if(xDistance > 0)
                        move = -1;
                    
                    (*water)[9][currentLocation] = '~';
                    currentLocation += move;
                    (*water)[9][currentLocation] = '^';
                }
                /*bool right = false;
                 if(xDistance < 0) {
                    xDistance *= -1;
                    right = true;
                }
                if(xDistance <= (9-i)) {
                    int move = -1;
                    if(right) move = 1;
                    moveFish(currentLocation, move, water);
                    currentLocation++;
                    break;
                }*/
            }
        }
        
        kill(getppid(), SIGUSR1);
        printf("Fish waiting for mill\n");
        sigsuspend(&mill_mask);
    }
    
    shmdt(water);
    printf("Fish done\n");
    exit(0);
}

int scanRow(int currentRow, int fish_x) {
    /*int minCol, maxCol;
     if(fish_x == 0) {
     minCol = 0;
     maxCol = minCol + currentRow;
     } else if(fish_x == rows-1) {
     maxCol = cols-1;
     minCol = maxCol - currentRow;
     } else {
     minCol = fish_x - currentRow;
     maxCol = fish_x + currentRow;
     }
     
     for(int col = minCol; col <= maxCol; col++) {
     if(arr[currentRow][col] == 'o')
     return col;
     }*/
    for(int col = 0; col < cols; col++) {
        if((*water)[currentRow][col] == 'o')
            return col;
    }
    return -1;
}