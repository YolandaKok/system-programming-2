//
// Created by yolanda on 27/3/2019.
//

#ifndef SYSTEM_PROGRAMMING_2_READPROCESS_H
#define SYSTEM_PROGRAMMING_2_READPROCESS_H

void readProcess(int id, struct dirent *dir, char *log_file);
void writeLogFile(char *log_file, char *filename, int bytes);
#endif //SYSTEM_PROGRAMMING_2_READPROCESS_H
