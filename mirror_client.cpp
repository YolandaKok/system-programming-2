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
#include "Map.h"
#include "SignalHandling.h"
#include "LinkedList.h"
#include "dfs_directories.h"
#include "readProcess.h"
#include "writeProcess.h"

void find_active_clients(char *common_dir, LinkedList *active_clients_list);
void client_has_exited(LinkedList *active_client_list, LinkedList *list, char *common_dir, char *mirror_dir);
void create_childs_for_copy(int id, struct dirent *dir, char *input_dir, char *log_file, char *common_dir, int buffer_size, Map *pids_map, char *mirror_dir);
void create_childs_for_copy2(int id, char *current_id, char *input_dir, char *log_file, char *common_dir, int buffer_size, Map *pids_map, char *mirror_dir);

int child_exited = 0;
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
    sigaction(SIGCHLD, &act, NULL);
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
    struct dirent *dir, *dir1;
    struct stat s, s1;
    // Create a list to keep the ids
    LinkedList *list = new LinkedList();
    // Map for pids
    Map *pids_map = new Map();
    while(1) {

        /* Open the directory and find if some client has left the network */
        LinkedList *active_clients_list = new LinkedList();
        find_active_clients(common_dir, active_clients_list);

        client_has_exited(active_clients_list, list, common_dir, mirror_dir);

        delete active_clients_list;

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
                    int found = list->find(dir->d_name);
                    ListNode *node;
                    if(found == 0 && (atoi(dir->d_name) != id) && ok == 1) {
                        list->add(dir->d_name);
                        node = list->getItemById(dir->d_name);
                    }
                    else if(found == 1) {
                        node = list->getItemById(dir->d_name);
                    }
                    if ((atoi(dir->d_name) != id) && ok == 1) {
                        if(found == 0) {
                            create_childs_for_copy(id, dir, input_dir, log_file, common_dir, buffer_size, pids_map, mirror_dir);
                        }
                      }
                }
            }
            closedir(d);
        }

        if(done != 0) {
            writeLogFile(log_file, NULL, 0, 4, 0);
            delete list;
            delete pids_map;
            /* Deallocate Memory */
            free(input_dir); free(log_file);
            exit_client(common_dir, mirror_dir, id);
        }
        if(child_exited != 0) {
            int stat;
            pid_t pid;
            pid = waitpid(-1, &stat, WNOHANG);
            if(pid > 0) {
                printf("Child with pid: %d terminated, Status code: %d\n", pid, WEXITSTATUS(stat));
                if(WEXITSTATUS(stat) == 1) {
                    printf("Read 00 and exited normally for client input %s !\n", pids_map->find(pid));
                    list->update(pids_map->find(pid), 1);
                }
                if(WEXITSTATUS(stat) == 2) {
                    printf("Error on read process !\n");
                    // Num of tries
                    /*list->update(pids_map->find(pid), 0);
                    if(list->getItemById(pids_map->find(pid))->getTries() < 3) {
                        create_childs_for_copy2(id, pids_map->find(pid), input_dir, log_file, common_dir, buffer_size, pids_map, mirror_dir);
                    }*/
                }
            }
        }
    }
}

void create_childs_for_copy(int id, struct dirent *dir, char *input_dir, char *log_file, char *common_dir, int buffer_size, Map *pids_map, char *mirror_dir) {
    pid_t pid, pid1;
    printf("%s\n", dir->d_name);
    // Fork two processes
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
            writeProcess(id, atoi(dir->d_name), input_dir, log_file, common_dir, buffer_size);
        }
        default:
        {
            pids_map->add(pid, dir->d_name);
            // Parent Process
            switch(pid1 = fork()) {
                case -1:
                    perror("fork error");
                    exit(5);
                case 0: // Child Process for read
                {
                    readProcess(id, atoi(dir->d_name), log_file, common_dir, mirror_dir, buffer_size, pid);
                }
                default:
                    pids_map->add(pid1, dir->d_name);
            }
            break;
        }
    }
}

void create_childs_for_copy2(int id,  char *current_id, char *input_dir, char *log_file, char *common_dir, int buffer_size, Map *pids_map, char *mirror_dir) {
    pid_t pid, pid1;
    //printf("%s\n", current_id);
    // Fork two processes
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
            writeProcess(id, atoi(current_id), input_dir, log_file, common_dir, buffer_size);
        }
        default:
        {
            pids_map->add(pid, current_id);
            // Parent Process
            switch(pid1 = fork()) {
                case -1:
                    perror("fork error");
                    exit(5);
                case 0: // Child Process for read
                {
                    readProcess(id, atoi(current_id), log_file, common_dir, mirror_dir, buffer_size, pid);
                }
                default:
                    pids_map->add(pid1, current_id);
            }
            break;
        }
    }
}

void find_active_clients(char *common_dir, LinkedList *active_clients_list) {
    DIR *d1;
    struct dirent *dir1;

    d1 = opendir(common_dir);
    int ok;
    if(d1) {
        while((dir1 = readdir(d1)) != NULL) {
            if(dir1->d_name[strlen(dir1->d_name)-1] == 'd') {
                active_clients_list->add(dir1->d_name);
            }
        }
    }
    closedir(d1);
}

void client_has_exited(LinkedList *active_clients_list, LinkedList *list, char *common_dir, char *mirror_dir) {
    /* Check if everything in list 2 is also in list 1 */
    for(int k = 0; k < list->length(); k++) {
        if(active_clients_list->find(list->getItem(k))) {

        }
        else {
            printf("Client with id: %s has left the network\n", list->getItem(k));
            char *item = list->getItem(k);
            int id_item = atoi(list->getItem(k));
            list->deleteItem(item);
            pid_t pid2;
            pid2 = fork();
            if(pid2 == 0) {
                char *buffer = (char*)malloc(strlen(mirror_dir) + numOfDigits(id_item) + 4);
                sprintf(buffer, "%s/%d", mirror_dir, id_item);
                printf("Deleting folder: %s\n", buffer);
                execl("/bin/rm", "rm", "-rf", buffer, NULL) ;
                free(buffer);
            }
        }
    }
}
