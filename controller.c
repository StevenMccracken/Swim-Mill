//
//  controller.c
//  
//
//  Created by Steven McCracken on 4/10/16.
//
//

#include "controller.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t client, server;
    
    client = fork();
    if(client == 0) {
        static char *argv[] = {"","",NULL};
        execv("/Users/stevenmccracken/Documents/GitHub/School/Swim-Mill/client", argv);
        exit(127);
    } else {
        //waitpid(client, 0, 0);
    }
    server = fork();
    if(server == 0) {
        static char *argv[] = {"","",NULL};
        execv("/Users/stevenmccracken/Documents/GitHub/School/Swim-Mill/server", argv);
        exit(127);
    } else {
        //waitpid(server, 0, 0);
    }
    return 0;
}