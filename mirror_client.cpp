#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IOutils.h"
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include<fcntl.h>
#include <unistd.h>
#include <math.h>
#include <sys/inotify.h>
#include <dirent.h>
#include <sys/stat.h>
#include <wait.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

int main(int argc, char *argv[]) {
    int id, buffer_size;
    char *common_dir = NULL, *input_dir = NULL, *mirror_dir = NULL, *log_file = NULL;
    // Read the arguments
    readArgs(argc, argv, id, common_dir, input_dir, mirror_dir, buffer_size, log_file);
    printf("%d %s %s %s %s %d\n", id, common_dir, input_dir, mirror_dir, log_file, buffer_size);
    /* Check if input dir exists */
    if(opendir(input_dir) == NULL) {
        printf("Input Directory does not exist !\n");
        exit(1);
    }
    if(opendir(mirror_dir) != NULL) {
        /* The directory exist */
        printf("Mirror Directory already exist !\n");
        exit(2);
    }
    else {
        /* Create mirror_dir */
        mkdir(mirror_dir, 0777);
    }
    if(opendir(common_dir) == NULL) {
        mkdir(common_dir, 0777);
    }

    char buffer[50];

    sprintf(buffer, "./common/%d.id", id);
    /* Write file with the id to the common dir */
    int fd;
    fd = open(buffer, O_RDWR|O_CREAT, 0777);
    char buffer1[4];
    /* Write inside this file */
    sprintf(buffer1, "%d", getpid());
    write(fd, buffer1, 4);
    close(fd);

    DIR *d;
    pid_t pid, pid1;
    d = opendir(common_dir);
    struct dirent *dir;
    struct stat s;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (stat(dir->d_name,&s) != 0) {
                /* is this a regular file? */
                printf("%s\n", dir->d_name);
            }
        }
        closedir(d);
    }

    /* Keep the file names */
    int status;
    switch(pid = fork()) {
        case -1:
            perror("fork error");
            exit(4);
        case 0:
            printf("Child Process For Read %d\n", getpid());
            // TODO: exec
            // TODO: Create a FIFO
            break;
        default:
        {
            wait(&status);
            printf("Parent Process\n");
            switch(pid1 = fork()) {
                case -1:
                    perror("fork error");
                    exit(5);
                case 0:
                    printf("Child Process2 for write to pipe %d\n", getpid());
                    // TODO: Create a FIFO
                    // TODO: exec
                    break;
                default:
                    wait(&status);
                    printf("Parent Process\n");
            }
        }
    }

    /* Deallocate Memory */
    free(common_dir); free(input_dir); free(mirror_dir); free(log_file);
}