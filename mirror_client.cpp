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
    fd = open(buffer, O_RDWR|O_CREAT|O_TRUNC, 0777);
    char buffer1[4];
    /* Write inside this file */
    sprintf(buffer1, "%d", getpid());
    write(fd, buffer1, 5);
    close(fd);

    DIR *d;
    pid_t pid, pid1;
    d = opendir(common_dir);
    struct dirent *dir;
    struct stat s;
    char* ids[5];
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (stat(dir->d_name,&s) != 0) {
                /* is this a regular file? */
                printf("%s\n", dir->d_name);
                ids[0] = dir->d_name;
                if(atoi(ids[0]) == 0) {
                    ids[0] = "2";
                    break;
                }
            }
        }
        closedir(d);
    }

    /* For every untraced file make a fifo */
    printf("Parent Process \n");
    /* Keep the file names */
    int status;
    switch(pid = fork()) {
        case -1:
            perror("fork error");
            exit(4);
        case 0:
            {   printf("Child Process2 for write to pipe %d\n", getpid());
                char buffer4[80];
                sprintf(buffer4, "common/id%d_to_id%d.fifo", id, atoi(ids[0]));
                mkfifo(buffer4, 0777);
                int fd6 = open(buffer4, O_WRONLY | O_CREAT, 0777);
                // Find the files that you want to send
                DIR *d1;
                d1 = opendir(input_dir);
                struct dirent *dir1;
                struct stat s1;
                char arr1[50];
                if (d1) {
                    while ((dir1 = readdir(d1)) != NULL) {
                        if (stat(dir1->d_name,&s1) != 0) {
                            printf("%s Send\n", dir1->d_name);
                            sprintf(arr1, "%hu", strlen(dir1->d_name));
                            write(fd6, arr1, 2);
                            strcpy(arr1, dir1->d_name);
                            write(fd6, arr1, strlen(dir1->d_name));
                        }
                    }
                    strcpy(arr1, "00");
                    write(fd6, arr1, 2);
                    closedir(d1);
                }
                exit(0);
                break;
            }
        default:
        {
            // Parent Process
            switch(pid1 = fork()) {
                case -1:
                    perror("fork error");
                    exit(5);
                case 0:
                    {
                        printf("Child Process For Read %d\n", getpid());
                        char buffer3[80];
                        sprintf(buffer3, "common/id%d_to_id%d.fifo", atoi(ids[0]), id);
                        mkfifo(buffer3, 0777);
                        int fd5 = open(buffer3, O_RDONLY | O_CREAT, 0777);
                        int nread;
                        char str1[50];
                        //do {
                            while((nread = read(fd5, str1, 50)) > 0) {
                                printf("%c Received\n", str1[0]);
                                for( int i = 0; i < nread; i++ )
                                    printf("Char%d: %c\n", i, str1[i]);
                                printf("%d \n", nread);
                            }
                       // }while(strcmp(str1, "00") != 0);
                       // exit(0);
                    }
                default:
                    // Parent Process
                    printf("Parent Process\n");
                    wait(&status);
                    break;
            }
            break;
        }
    }
    wait(&status);

    /* Deallocate Memory */
    free(common_dir); free(input_dir); free(mirror_dir); free(log_file);
}