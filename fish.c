#include "includes.h"

void catchKillSig();
void catchEndSig();
void move(int);
int* locatePellets(int);

int currentLocation = rows/2;

int main() {
    printf("PID %d (fish) started\n", getpid());
    
    // Setup catch for kill signal from swim_mill
    signal(SIGINT, catchKillSig);
    signal(SIGUSR1, catchEndSig);
    
    // Attach process to shared memory
    attachSharedMemory();
    
    while(1) {
        bool moveTowardsCenter = false;
        
        // Start from bottom and go to top, checking all rows for pellets
        for(int row = (rows-2); row >= 0; row--) {
            // Locate any pellets on the current row
            int* pelletLocations = locatePellets(row);
            
            /* If there is a pellet directly above the fish, don't move
             and don't check any other rows so the fish can eat it */
            if(row == (rows-2) && *(pelletLocations+currentLocation) == 1) {
                sleep(1);
                break;
            } else { // Else, there isn't a pellet directly above the fish
                int closestPellet = -1;
                
                /* Find the closest pellet in the row if the fish can
                 even reach that space by the time the pellet falls out*/
                for(int col = 0; col < cols; col++) {
                    if(*(pelletLocations+col) == 1) {
                        closestPellet = col;
                        break;
                    }
                }
                
                // If there is a pellet in the row that the fish can reach
                if(closestPellet != -1) {
                    
                    /* Move left or right depending on where the
                     pellet is in relation to the fish */
                    if(closestPellet > currentLocation)
                        move(1);
                    else
                        move(-1);
                    
                    sleep(1);
                    break; // Stop checking higher rows and start check over again
                } else if(row == 0)
                    moveTowardsCenter = true;
            }
        }
        
        /* If the check for pellets reached the highest row
         and no pellets were reachable */
        if(moveTowardsCenter) {
            if(currentLocation > cols/2)
                move(-1);
            else
                move(1);
        }
    }
}

void catchKillSig() {
    shmdt(water);
    printf("PID %d (fish) killed because of ^C\n", getpid());
    exit(0);
}

void catchEndSig() {
    shmdt(water);
    printf("PID %d (fish) killed because time limit reached\n", getpid());
    exit(0);
}

void move(int direction) {
    (*water)[9][currentLocation] = '~';
    currentLocation += direction;
    (*water)[9][currentLocation] = '^';
}

int* locatePellets(int currentRow) {
    int pelletLocations[10] = {0};
    int range, start, end;
    
    /* For a given row, the fish can only reach +/-
     ((totalRows - 1) - currentRow) spaces away from it */
    range = ((rows - 1) - currentRow);
    
    /* If the range to the left of the fish is out
     out of bounds, just set the min left range to 0 */
    start = currentLocation - range;
    if(start < 0) start = 0;
    
    /* If the range to the right of the fish is out
     out of bounds, just set the min left range to last col */
    end = currentLocation + range;
    if(end >= cols) end = (cols - 1);
    
    // Start from min reachable range (left) from fish
    for(int i = start; i < currentLocation; i++) {
        if((*water)[currentRow][i] == 'o')
            pelletLocations[i] = 1;
    }
    
    // Start from fish position to max range (right)
    for(int i = currentLocation; i <= end; i++) {
        if((*water)[currentRow][i] == 'o')
            pelletLocations[i] = 1;
    }
    
    /* Array will be all 0's, except where a pellet exists
     and the fish can reach it, then that index will be a 1 */
    return pelletLocations;
}