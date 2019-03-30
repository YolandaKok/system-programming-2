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
#include <errno.h>

void readProcess(int id, struct dirent *dir, char *log_file) {
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
            int b;
            read(fd5, &b, sizeof(b));
            printf("%d INTTT\n", b);
            char buffer[200];
            int chunks;
            if(b % 200 != 0) {
                chunks = (b / 200) + 1;
            }
            else {
                chunks = (b/200);
            }
            char buffer5[100];
            sprintf(buffer5, "%d.mirror/%d%s", id, id2, str1);
            FILE *fp = fopen(buffer5, "w");
            for(int j = 0; j < chunks; j++) {
                memset(buffer, 0, sizeof(buffer));
                read(fd5, buffer, 200);
                fprintf(fp, "%s", buffer);
            }

            /* Write to the file */
            writeLogFile(log_file, str1, b);
        }
        else {
            read(fd5, str1, number);
            str1[number] = '\0';
            printf("Name LALA%s\n", str1);
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
                mkdir(buffer5, 0777);
                closedir(dir);
            }
            else if (ENOENT == errno)
            {
                mkdir(buffer4, 0777);
                char buffer5[80];
                sprintf(buffer5, "%d.mirror/%d%s", id, id2, str1);
                mkdir(buffer5, 0777);
            }
        }

        // If does not exists just create it
        // create the file and put it inside
    }while(!(str2[0] == '0' && str2[1] == '0'));
    exit(0);
}


void writeLogFile(char *log_file, char *filename, int bytes) {
    /* Open the log file and create */
    /*int fd = open(log_file, O_WRONLY | O_APPEND | O_CREAT, 0660);
    char buffer[100];
    sprintf(buffer, "Receive %s %d\n", filename, bytes);
    write(fd, buffer, sizeof(buffer));
    memset(buffer, 0, sizeof(buffer));
    close(fd);*/
    FILE *fp = fopen(log_file, "a");
    fprintf(fp, "%s %s %d\n", "Received", filename, bytes);
    fclose(fp);
}