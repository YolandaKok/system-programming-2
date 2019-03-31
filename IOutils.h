//
// Created by Music on 25/3/2019.
//

#ifndef SYSTEM_PROGRAMMING_2_IOUTILS_H
#define SYSTEM_PROGRAMMING_2_IOUTILS_H

int readArgs(int argc, char* argv[], int& id, char*& common_dir, char*& input_dir,
             char*& mirror_dir, int& buffer_size, char*& log_file);

void writeLogFile(char *log_file, char *filename, int bytes, int mode, int id);

void catchinterrupt (int signo);

#endif //SYSTEM_PROGRAMMING_2_IOUTILS_H
