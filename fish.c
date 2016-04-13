//
//  fish.c
//  
//
//  Created by Steven McCracken on 4/9/16.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSZ   27
int rows = 10;
int cols = 10;
int scanRow(int, int, *char);

int main() {
    int shmid;
    key_t key = 1738;
    char (*water)[rows][cols];
    
    // Locate segment
    if((shmid = shmget(key, sizeof(water), 0666)) < 0) {
        perror("shmget");
        exit(1);
    } else {
        //printf("Fish located segment %d\n", shmid);
    }
    
    // Attach segment to data space
    if((water = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    } else {
        //printf("Fish attached segment to data space %p\n", water);
    }
    
    int pelletLocations[20][2];
    int numPellets = 0;
    while(1) {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                if((*water)[i][j] == 'o') {
                    pelletLocations[numPellets][0] = i;
                    pelletLocations[numPellets][1] = j;
                    numPellets++;
                }
            }
        }
        sleep(1);
        for(int i = 0; i < 20; i++) {
            for(int j = 0; j < 2; j++) {
                printf("%d",pelletLocations[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        sleep(1);
    }
    printf("Fish done\n");
    exit(0);
}

int scanRow(int currentRow, int fish_x, *char water) {
    int minCol, maxCol;
    if(fish_x == 0) {
        minCol = 0;
        maxCol = minCol + currentRow;
    } else if(fish_x == rows-1) {
        maxCol = rows-1;
        minCol = maxCol - rows;
    } else {
        minCol = fish_x - currentRow;
        maxCol = fish_x + rows;
    }
    
    for(int col = minCol; col <= maxCol; col++) {
        if((*water)[currentRow][col] == 'o')
            return col;
    }
    return -1;
}
