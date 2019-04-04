//
// Created by yolanda on 27/3/2019.
//

#include <sys/types.h>

#ifndef SYSTEM_PROGRAMMING_2_WRITEPROCESS_H
#define SYSTEM_PROGRAMMING_2_WRITEPROCESS_H

void writeProcess(int id, struct dirent *dir, char *input_dir, char *log_file, char *common_dir, int buffer_size);
ssize_t writeall(int fd, const void *buf, size_t nbyte);

#endif //SYSTEM_PROGRAMMING_2_WRITEPROCESS_H
