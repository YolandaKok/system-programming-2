//
// Created by yolanda on 6/4/2019.
//

#ifndef SYSTEM_PROGRAMMING_2_MAPNODE_H
#define SYSTEM_PROGRAMMING_2_MAPNODE_H

class MapNode {
private:
    pid_t key;
    char *client;
    MapNode *next;
public:
    MapNode();
    MapNode(pid_t key, char *client_id);
    void add(pid_t key, char *client, MapNode *head);
    char* find(pid_t key, MapNode *head);
    MapNode* getNext();
    ~MapNode();
};

#endif //SYSTEM_PROGRAMMING_2_MAPNODE_H
