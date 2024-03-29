//
// Created by yolanda on 28/3/2019.
//

#ifndef SYSTEM_PROGRAMMING_2_DFS_DIRECTORIES_H
#define SYSTEM_PROGRAMMING_2_DFS_DIRECTORIES_H

void listdir(const char *name, int level, LinkedList *list);
int isRegular(char *path);
void removeInputDirectory(char *path, char *input_prefix);
void removeInputDirectoryFromList(char *input_prefix, LinkedList *list);

#endif //SYSTEM_PROGRAMMING_2_DFS_DIRECTORIES_H
