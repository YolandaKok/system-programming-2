//
// Created by yolanda on 27/3/2019.
//

#include <sys/types.h>

#ifndef SYSTEM_PROGRAMMING_2_READPROCESS_H
#define SYSTEM_PROGRAMMING_2_READPROCESS_H

void readProcess(int id, struct dirent *dir, char *log_file, int buffer_size);
ssize_t readall(int fd, void *buf, size_t nbyte);

#endif //SYSTEM_PROGRAMMING_2_READPROCESS_H
