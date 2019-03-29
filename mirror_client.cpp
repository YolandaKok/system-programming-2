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
#include <errno.h>
#include <wait.h>
#include "LinkedList.h"
#include "dfs_directories.h"

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
    struct dirent *dir;
    struct stat s;
    // Create a list to keep the ids
    LinkedList *list = new LinkedList();

    while(1) {
        d = opendir(common_dir);
        // Find the directories
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if (stat(dir->d_name, &s) != 0) {
                    int ok;
                    if(dir->d_name[strlen(dir->d_name)-1] == 'd')
                        ok = 1;
                    else
                        ok = 0;
                    /* is this a regular file? */
                    int found = list->find(dir->d_name);
                    if (found == 0 && (atoi(dir->d_name) != id) && ok == 1) {
                        printf("%s\n", dir->d_name);
                        // Fork two processes
                        list->add(dir->d_name);
                        printf("Parent Process \n");
                        int status;
                        switch(pid = fork()) {
                            case -1:
                                perror("fork error");
                                exit(4);
                            case 0:
                            {
                                printf("Child Process2 for write to pipe %d\n", getpid());
                                char buffer4[80];
                                sprintf(buffer4, "common/id%d_to_id%d.fifo", id, atoi(dir->d_name));
                                mkfifo(buffer4, 0777);
                                int fd6 = open(buffer4, O_WRONLY | O_CREAT, 0777);
                                // Find the files that you want to send
                                char arr1[50];
                                LinkedList *list = new LinkedList();
                                listdir(input_dir, 0, list);
                                /* Remove the prefix from all */
                                removeInputDirectoryFromList(input_dir, list);
                                printf("%d list length\n", list->length());
                                /* List Contains all the files */
                                for(int i = 0; i < list->length(); i++) {
                                    /* Send if it is regular or not */
                                    int regular = list->getListNodeItem(i)->getRegular();
                                    char file[2];
                                    sprintf(file, "%hu", regular);
                                    write(fd6, file, 2);
                                    // Send the list of files and directories
                                    printf("%s Send\n", list->getItem(i));
                                    sprintf(arr1, "%hu", strlen(list->getItem(i)));
                                    if(strlen(list->getItem(i)) <= 9) {
                                        arr1[0] = '0';
                                        sprintf(&arr1[1], "%hu", strlen(list->getItem(i)));
                                    }
                                    write(fd6, arr1, 2);
                                    strcpy(arr1, list->getItem(i));
                                    write(fd6, arr1, strlen(list->getItem(i)));
                                }
                                printf("xexexexe\n");
                                int regular = 2;
                                char file[2];
                                sprintf(file, "%hu", regular);
                                write(fd6, file, 2);
                                char arrr[2];
                                strcpy(arrr, "00");
                                write(fd6, arrr, 2);
                                exit(0);
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
                                        sprintf(buffer3, "common/id%d_to_id%d.fifo", atoi(dir->d_name), id);
                                        int id2 = atoi(dir->d_name);
                                        mkfifo(buffer3, 0777);
                                        int fd5 = open(buffer3, O_RDONLY | O_CREAT, 0777);
                                        int nread, nread1;
                                        char str1[50];
                                        char str2[2];
                                        char file[2];
                                        int count = 0;
                                        do {
                                            read(fd5, file, 2);
                                            int f = file[0] - '0';
                                            printf("%d xxxxxx\n", f);
                                            nread1 = read(fd5, str2, 2);
                                            printf("Length of the filename: %c %c\n", str2[0], str2[1]);
                                            int x1 = str2[0] - '0';
                                            int x2 = str2[1] - '0';
                                            printf("%d x1 %d x2\n", x1, x2);
                                            int number = x1 * 10 + x2;
                                            printf("%d NUMBER\n", number);
                                            if(str2[0] == '0' && str2[1] == '0')
                                                break;

                                            if(f) {
                                                // Read the name of the file or directory
                                                read(fd5, str1, number);
                                                str1[number] = '\0';
                                                printf("Name %s\n", str1);
                                                // Let's create the file
                                                char buffer4[80];
                                                sprintf(buffer4, "%d.mirror/%d", id, atoi(dir->d_name));
                                                printf("%s xixi\n", buffer4);
                                                // Check if directory exists
                                                DIR* dir = opendir(buffer4);
                                                if (dir)
                                                {
                                                    char buffer5[80];
                                                    sprintf(buffer5, "%d.mirror/%d%s", id, id2, str1);
                                                    printf("Lolo: %s\n", buffer5);
                                                    open(buffer5, O_WRONLY | O_APPEND | O_CREAT, 0777);
                                                    closedir(dir);
                                                }
                                                else if (ENOENT == errno)
                                                {
                                                    mkdir(buffer4, 0777);
                                                    char buffer5[80];
                                                    sprintf(buffer5, "%d.mirror/%d%s", id, id2, str1);
                                                    printf("Loli: %s\n", buffer5);
                                                    open(buffer5, O_WRONLY | O_APPEND | O_CREAT, 0777);
                                                }
                                            }
                                            else {
                                                read(fd5, str1, number);
                                                str1[number] = '\0';
                                                printf("Name %s\n", str1);
                                                char buffer5[80];
                                                sprintf(buffer5, "%d.mirror/%d%s", id, id2, str1);
                                                mkdir(buffer5, 0777);
                                            }
                                            count++;
                                            // If does not exists just create it
                                            // create the file and put it inside
                                        }while(!(str2[0] == '0' && str2[1] == '0'));
                                        exit(0);
                                    }
                                    default:
                                        int stat;
                                        pid_t pid;
                                        // Parent Process
                                        printf("Parent Process xixi\n");
                                        pid = waitpid(-1, &stat, WNOHANG);
                                        if(pid > 0)
                                            printf("child %d terminated\n", pid);
                                        break;
                                }
                                break;
                            }
                        }
                      }
                }
            }
            closedir(d);
        }
        int stat;
        pid_t pid;
        pid = waitpid(-1, &stat, WNOHANG);
        if(pid > 0)
            printf("child %d terminated\n", pid);
    }

    /* For every untraced file make a fifo */
/*
    printf("Parent Process \n");
    int status;
    switch(pid = fork()) {
        case -1:
            perror("fork error");
            exit(4);
        case 0:
            {
                writeProcess();
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
                        readProcess();
                        break;
                    }
                default:
                    int stat;
                    pid_t pid;
                    // Parent Process
                    printf("Parent Process xixi\n");
                    pid = waitpid(-1, &stat, WNOHANG);
                    if(pid > 0)
                        printf("child %d terminated\n", pid);
                    break;
            }
            break;
        }
    }
    wait(&status);
*/
    /* Deallocate Memory */
    free(common_dir); free(input_dir); free(mirror_dir); free(log_file);
}