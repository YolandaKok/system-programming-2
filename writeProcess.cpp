//
// Created by yolanda on 27/3/2019.
//

#include "readProcess.h"
#include "writeProcess.h"
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
#include <errno.h>
#include "dfs_directories.h"
#include "IOutils.h"

void writeProcess(int id, int id2, char *input_dir, char *log_file, char *common_dir, int buffer_size) {
    printf("Child Process2 for write to pipe %d\n", getpid());
    char buffer4[80];
    sprintf(buffer4, "%s/id%d_to_id%d.fifo", common_dir, id, id2);
    mkfifo(buffer4, 0777);
    int fd6 = open(buffer4, O_WRONLY | O_CREAT, 0777);
    // Find the files that you want to send
    char arr1[50];
    LinkedList *list = new LinkedList();
    listdir(input_dir, 0, list);
    /* Remove the prefix from all */
    removeInputDirectoryFromList(input_dir, list);
    /* List Contains all the files */
    for(int i = 0; i < list->length(); i++) {
        /* Send if it is regular or not */
        int regular = list->getListNodeItem(i)->getRegular();
        char file[2];
        sprintf(file, "%hu", regular);
        writeall(fd6, file, 2);
        // Send the list of files and directories
        short int size1 = strlen(list->getItem(i));
        sprintf(arr1, "%hu", size1);
        if(strlen(list->getItem(i)) <= 9) {
            arr1[0] = '0';
            short int size2 = strlen(list->getItem(i));
            sprintf(&arr1[1], "%hu", size2);
        }
        writeall(fd6, arr1, 2);
        /* Write the path to the file */
        strcpy(arr1, list->getItem(i));
        writeall(fd6, arr1, strlen(list->getItem(i)));
        /* If it is regular write the length of the data */
        if(regular) {
            char buff[100];
            sprintf(buff, "%s%s", input_dir, list->getItem(i));
            FILE *fp = fopen(buff, "r");
            fseek(fp, 0L, SEEK_END);
            int sz = ftell(fp);
            fclose(fp);
            writeall(fd6, &sz, sizeof(sz));
            /* Loop to write the whole file */
            char buffer[buffer_size];
            int chunks = sz / buffer_size;
            int bytesRead;
            if(sz > 0) {
                fp = fopen(buff, "r");
                if (fp != NULL)
                {
                    // Process File
                    for(int j = 0; j < chunks; j++) {
                        fread(buffer, 1, buffer_size, fp);
                        writeall(fd6, buffer, buffer_size);
                    }
                    int remaining_bytes = sz - chunks * buffer_size;
                    char *remain_buffer = (char*)malloc(remaining_bytes);
                    fread(remain_buffer, 1, remaining_bytes, fp);
                    writeall(fd6, remain_buffer, remaining_bytes);
                    free(remain_buffer);
                }
                fclose(fp);
            }
            /* Open the file */
            /* Write the data to the file */
            writeLogFile(log_file, arr1, sz, 3, 0);
        }
    }
    int regular = 2;
    char file[2];
    sprintf(file, "%hu", regular);
    writeall(fd6, file, 2);
    char arrr[2];
    strncpy(arrr, "00", 2);
    unlink(buffer4);
    writeall(fd6, arrr, 2);
    delete list;
    exit(0);
}

/* Source: Programming with Unix. Author: Marc J. Rochkind. page 117 */
ssize_t writeall(int fd, const void *buf, size_t nbyte) {
    ssize_t nwritten = 0, n;

    do {
        if((n = write(fd, &((const char *)buf)[nwritten], nbyte - nwritten)) == -1) {
            if (errno == EINTR)
                continue;
            else
                return -1;
        }
        nwritten += n;
    } while(nwritten < nbyte);
    return nwritten;
}