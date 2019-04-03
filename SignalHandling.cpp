#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include "SignalHandling.h"

extern int done;

void catchinterrupt (int signo) {
    done = 1;
}

void exit_client(char *mirror_dir, int id) {
    pid_t pid;
    pid = fork();
    if(pid == 0) {
        execl("/bin/rm", "rm", "-rf", mirror_dir, NULL) ;
    }
    else {
        char buffer2[50];
        sprintf(buffer2, "common/%d.id", id);
        pid_t pid2;
        pid2 = fork();
        if(pid2 == 0) {
            execl("/bin/rm", "rm", buffer2, NULL);
        }
        else {
            int status;
            wait(&status);
            free(mirror_dir);
            exit(0);
        }
    }
}