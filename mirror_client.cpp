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
#include <signal.h>
#include "Tools.h"
#include "SignalHandling.h"
#include "LinkedList.h"
#include "dfs_directories.h"
#include "readProcess.h"
#include "writeProcess.h"

int done = 0;

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

    /* Initialize signal handler */
    static struct sigaction act;
    act.sa_handler = catchinterrupt;
    sigfillset(&( act.sa_mask));
    sigaction(SIGINT, &act , NULL);
    sigaction(SIGQUIT, &act, NULL);
    sigaction(SIGUSR1, &act, NULL);
    /* End of signal handler initialization */
    writeLogFile(log_file, NULL, 0, 1, id);

    /* Allocate Buffer for string */
    char *buffer = (char*)malloc(strlen(common_dir) + numOfDigits(id) + 6);
    sprintf(buffer, "%s/%d.id", common_dir, id);
    /* Write file with the id to the common dir */
    FILE *fd;
    fd = fopen(buffer, "w");
    free(buffer);
    /* Write inside this file */
    fprintf(fd, "%d", getpid());
    fclose(fd);

    DIR *d, *d1;
    pid_t pid, pid1;
    struct dirent *dir, *dir1;
    struct stat s, s1;
    // Create a list to keep the ids
    LinkedList *list = new LinkedList();

    while(1) {
        /* Open the directory and find if some client has left the network */
        LinkedList *list2 = new LinkedList();
        d1 = opendir(common_dir);
        int ok;
        if(d1) {
            while((dir1 = readdir(d1)) != NULL) {
                if(dir1->d_name[strlen(dir1->d_name)-1] == 'd') {
                    list2->add(dir1->d_name);
                }
            }
        }
        closedir(d1);

        /* Check if everything in list 2 is also in list 1 */
        for(int k = 0; k < list->length(); k++) {
            if(list2->find(list->getItem(k))) {

            }
            else {
                printf("Not Found\n");
                char *item = list->getItem(k);
                int id_item = atoi(list->getItem(k));
                list->deleteItem(item);
                pid_t pid2;
                pid2 = fork();
                if(pid2 == 0) {
                    printf("Item: %d\n", id_item);
                    printf("File: %s \n", mirror_dir);
                    //char buffer[100];
                    char *buffer = (char*)malloc(strlen(mirror_dir) + numOfDigits(id_item) + 4);
                    sprintf(buffer, "%s/%d", mirror_dir, id_item);
                    execl("/bin/rm", "rm", "-rf", buffer, NULL) ;
                    free(buffer);
                }
                break;
            }
        }

        delete list2;

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
                                writeProcess(id, dir, input_dir, log_file, common_dir, buffer_size);
                            }
                            default:
                            {
                                // Parent Process
                                switch(pid1 = fork()) {
                                    case -1:
                                        perror("fork error");
                                        exit(5);
                                    case 0: // Child Process for read
                                    {
                                        readProcess(id, dir, log_file, common_dir, mirror_dir, buffer_size, pid);
                                    }
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
        if(done != 0) {
            writeLogFile(log_file, NULL, 0, 4, 0);
            delete list;
            /* Deallocate Memory */
            free(input_dir); free(log_file);
            exit_client(common_dir, mirror_dir, id);
        }
    }
}