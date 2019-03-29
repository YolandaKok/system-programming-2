//
// Created by yolanda on 27/3/2019.
//

#include "readProcess.h"
#include "LinkedList.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "dfs_directories.h"

void readProcess(int id, struct dirent *dir, char *input_dir) {
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
    int regular = 2;
    char file[2];
    sprintf(file, "%hu", regular);
    write(fd6, file, 2);
    char arrr[2];
    strcpy(arrr, "00");
    write(fd6, arrr, 2);
    exit(0);
}