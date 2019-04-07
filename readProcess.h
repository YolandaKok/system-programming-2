//
// Created by yolanda on 27/3/2019.
//

#include <sys/types.h>

#ifndef SYSTEM_PROGRAMMING_2_READPROCESS_H
#define SYSTEM_PROGRAMMING_2_READPROCESS_H

void readProcess(int id, int id2, char *log_file, char *common_dir, char *mirror_dir, int buffer_size, pid_t pid);
ssize_t readall(int fd, void *buf, size_t nbyte);

#endif //SYSTEM_PROGRAMMING_2_READPROCESS_H
