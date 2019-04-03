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
#include "IOutils.h"

ssize_t readall(int fd, void *buf, size_t nbyte);

void readProcess(int id, struct dirent *dir, char *log_file, int buffer_size) {
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
    char *buffer = (char*)malloc((buffer_size + 1) * sizeof(char));
    do {
        readall(fd5, file, 2);
        int f = file[0] - '0';
        //printf("%d xxxxxx\n", f);
        nread1 = readall(fd5, str2, 2);
        //printf("Length of the filename: %c %c\n", str2[0], str2[1]);
        int x1 = str2[0] - '0';
        int x2 = str2[1] - '0';
        //printf("%d x1 %d x2\n", x1, x2);
        int number = x1 * 10 + x2;
        //printf("%d NUMBER\n", number);
        if(str2[0] == '0' && str2[1] == '0')
            break;

        if(f) {
            // Read the name of the file or directory
            readall(fd5, str1, number);
            str1[number] = '\0';
            //printf("Name %s\n", str1);
            // Let's create the file
            char buffer4[80];
            sprintf(buffer4, "%d.mirror/%d", id, atoi(dir->d_name));
            //printf("%s xixi\n", buffer4);
            // Check if directory exists
            DIR* dir = opendir(buffer4);
            if (dir)
            {
                char buffer5[80];
                sprintf(buffer5, "%d.mirror/%d%s", id, id2, str1);
                //printf("Lolo: %s\n", buffer5);
                open(buffer5, O_WRONLY | O_APPEND | O_CREAT, 0777);
                closedir(dir);
            }
            else if (ENOENT == errno)
            {
                mkdir(buffer4, 0777);
                char buffer5[80];
                sprintf(buffer5, "%d.mirror/%d%s", id, id2, str1);
               // printf("Loli: %s\n", buffer5);
                open(buffer5, O_WRONLY | O_APPEND | O_CREAT, 0777);
            }
            int b;
            readall(fd5, &b, sizeof(b));
            printf("%d INTTT\n", b);
            int chunks = b / buffer_size;
            /*if(b % 200 != 0) {
                chunks = (b / 200) + 1;
            }
            else {
                chunks = (b/200);
            }*/
            char buffer5[100];
            sprintf(buffer5, "%d.mirror/%d%s", id, id2, str1);
            FILE *fp = fopen(buffer5, "w");
            for(int j = 0; j < chunks; j++) {
                //memset(buffer, 0, sizeof(buffer));
                //int nread = read(fd5, buffer, buffer_size);
                int nread = readall(fd5, buffer, buffer_size);
                //printf("%d READ\n", nread);
                buffer[buffer_size]='\0';
                //printf("Yeah: %s\n\n", buffer);
                fprintf(fp, "%s", buffer);
            }
            int remaining_bytes = b - buffer_size * chunks;
            char *remain_buffer = (char*)malloc(remaining_bytes + 1);
            //read(fd5, remain_buffer, remaining_bytes);
            readall(fd5, remain_buffer, remaining_bytes);
            remain_buffer[remaining_bytes]='\0';
            fprintf(fp, "%s", remain_buffer);
            fclose(fp);
            free(remain_buffer);
            /* Write to the file */
            writeLogFile(log_file, str1, b, 2, 0);
        }
        else {
            read(fd5, str1, number);
            str1[number] = '\0';
            //printf("Name LALA%s\n", str1);
            char buffer4[80];
            sprintf(buffer4, "%d.mirror/%d", id, atoi(dir->d_name));
           // printf("%s xixi\n", buffer4);
            // Check if directory exists
            DIR* dir = opendir(buffer4);
            if (dir)
            {
                char buffer5[80];
                sprintf(buffer5, "%d.mirror/%d%s", id, id2, str1);
               // printf("Lolo: %s\n", buffer5);
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
    free(buffer);
    exit(0);
}

ssize_t readall(int fd, void *buf, size_t nbyte) {
    ssize_t nread = 0, n;

    do {
        if((n = read(fd, &((char*)buf)[nread], nbyte - nread)) == -1) {
            if(errno == EINTR)
                continue;
            else
                return -1;
        }
        if(n == 0)
            return nread;
        nread += n;
    }while(nread < nbyte);
    return nread;
}