#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include "SignalHandling.h"

extern int done;

void catchinterrupt (int signo) {
    if(signo == SIGINT || signo == SIGQUIT)
        done = 1;
    if(signo == SIGUSR1)
        printf("Child Error !\n");
}

void exit_client(char *common_dir, char *mirror_dir, int id) {
    pid_t pid;
    pid = fork();
    if(pid == 0) {
        execl("/bin/rm", "rm", "-rf", mirror_dir, NULL) ;
    }
    else {
        char buffer2[50];
        sprintf(buffer2, "%s/%d.id", common_dir, id);
        pid_t pid2;
        pid2 = fork();
        if(pid2 == 0) {
            execl("/bin/rm", "rm", buffer2, NULL);
        }
        else {
            int status;
            wait(&status);
            free(mirror_dir);
            free(common_dir);
            exit(0);
        }
    }
}