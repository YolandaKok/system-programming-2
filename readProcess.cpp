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
#include <signal.h>
#include "IOutils.h"

void readProcess(int id, int id2, char *log_file, char *common_dir, char *mirror_dir, int buffer_size, pid_t pid) {
    printf("Child Process For Read %d\n", getpid());
    char buffer3[80];
    sprintf(buffer3, "%s/id%d_to_id%d.fifo", common_dir, id2, id);
    //int id2 = atoi(dir->d_name);
    mkfifo(buffer3, 0777);
    int fd5 = open(buffer3, O_RDONLY | O_CREAT, 0777);
    if(fd5 < 0) {
        //kill(getppid(), SIGUSR1);
        kill(pid, SIGKILL);
        exit(2);
    }
    int nread, nread1;
    char str1[50];
    char str2[2];
    char file[2];
    int count = 0;
    char *buffer = (char*)malloc((buffer_size + 1) * sizeof(char));
    do {
        readall(fd5, file, 2);
        int f = file[0] - '0';
        nread1 = readall(fd5, str2, 2);
        int x1 = str2[0] - '0';
        int x2 = str2[1] - '0';
        int number = x1 * 10 + x2;
        if(str2[0] == '0' && str2[1] == '0')
            break;

        if(f) {
            // Read the name of the file or directory
            readall(fd5, str1, number);
            str1[number] = '\0';
            // Let's create the file
            char buffer4[80];
            sprintf(buffer4, "%s/%d", mirror_dir, id2);
            // Check if directory exists
            DIR* dir = opendir(buffer4);
            if (dir)
            {
                char buffer5[80];
                sprintf(buffer5, "%s/%d%s", mirror_dir, id2, str1);
                open(buffer5, O_WRONLY | O_APPEND | O_CREAT, 0777);
                closedir(dir);
            }
            else if (ENOENT == errno)
            {
                mkdir(buffer4, 0777);
                char buffer5[80];
                sprintf(buffer5, "%s/%d%s", mirror_dir, id2, str1);
                open(buffer5, O_WRONLY | O_APPEND | O_CREAT, 0777);
            }
            int b;
            readall(fd5, &b, sizeof(b));
            int chunks = b / buffer_size;
            char buffer5[100];
            sprintf(buffer5, "%s/%d%s", mirror_dir, id2, str1);
            FILE *fp = fopen(buffer5, "w");
            for(int j = 0; j < chunks; j++) {
                int nread = readall(fd5, buffer, buffer_size);
                if(nread < buffer_size) {
                  kill(pid, SIGKILL);
                  exit(2);
                }
                buffer[buffer_size]='\0';
                fprintf(fp, "%s", buffer);
            }
            int remaining_bytes = b - buffer_size * chunks;
            char *remain_buffer = (char*)malloc(remaining_bytes + 1);
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
            char buffer4[80];
            sprintf(buffer4, "%s/%d", mirror_dir, id2);
            // Check if directory exists
            DIR* dir = opendir(buffer4);
            if (dir)
            {
                char buffer5[80];
                sprintf(buffer5, "%s/%d%s", mirror_dir, id2, str1);
                mkdir(buffer5, 0777);
                closedir(dir);
            }
            else if (ENOENT == errno)
            {
                mkdir(buffer4, 0777);
                char buffer5[80];
                sprintf(buffer5, "%s/%d%s", mirror_dir, id2, str1);
                mkdir(buffer5, 0777);
            }
        }

        // If does not exists just create it
        // create the file and put it inside
    } while(!(str2[0] == '0' && str2[1] == '0'));
    free(buffer);
    unlink(buffer3);
    exit(1);
}

/* Use Read All function to catch the EINTR signal that may interrupt the read process */
/* Secures that nbytes will be read from read */
/* Source: Programming with Unix. Author: Marc J. Rochkind. page 118 */
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
