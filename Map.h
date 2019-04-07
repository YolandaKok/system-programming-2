//
// Created by yolanda on 6/4/2019.
//

#ifndef SYSTEM_PROGRAMMING_2_MAP_H
#define SYSTEM_PROGRAMMING_2_MAP_H

#include "MapNode.h"

class Map {
    private:
        int size;
        MapNode *head;
    public:
        Map();
        void add(pid_t pid, char *client_id);
        // Find client id
        char* find(pid_t pid);
        ~Map();
};

#endif //SYSTEM_PROGRAMMING_2_MAP_H
