//
// Created by yolanda on 28/3/2019.
//

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

int isRegular(char *path) {
    struct stat s1;
    stat(path, &s1);
    if(S_ISREG(s1.st_mode)) {
        return 1;
    }
    else {
        return 0;
    }
}

void listdir(const char *name, int level, LinkedList *list)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;
    if (!(entry = readdir(dir)))
        return;

    do {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            list->add(path, isRegular(path));
            listdir(path, level + 1, list);
        }
        else{
            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            list->add(path, isRegular(path));
        }
    } while (entry = readdir(dir));
    closedir(dir);
}


void removeInputDirectory(char *path, char *input_prefix) {
    int n = strlen(input_prefix);
    size_t len = strlen(path);
    if (n > len)
        return;  // Or: n = len;
    memmove(path, path+n, len - n + 1);
}


void removeInputDirectoryFromList(char *input_prefix, LinkedList *list) {
    for(int i = 0; i < list->length(); i++) {
        removeInputDirectory(list->getItem(i), input_prefix);
    }
}