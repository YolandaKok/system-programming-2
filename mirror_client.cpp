#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IOutils.h"
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <sys/stat.h>
#include <errno.h>
#include <wait.h>
#include "LinkedList.h"
#include "dfs_directories.h"
#include "readProcess.h"
#include "writeProcess.h"

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
                                /* Read From the input directory */
                                /* and write to pipe */
                                writeProcess(id, dir, input_dir);
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
                                        readProcess(id, dir);
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

    /* Deallocate Memory */
    free(common_dir); free(input_dir); free(mirror_dir); free(log_file);
}
