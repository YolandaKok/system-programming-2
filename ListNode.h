//
// Created by yolanda on 24/2/2019.
//

#ifndef SYSTEM_PROGRAMMING_1_LISTNODE_H
#define SYSTEM_PROGRAMMING_1_LISTNODE_H
#include <stdio.h>

class ListNode {
    private:
        ListNode *next;
        char *id;
    public:
        ListNode();
        ListNode(char* id);
        void insert(char* id, ListNode *head);
        ListNode* getNext();
        void print();
        int getBalance();
        int find(char *id, ListNode *head);
        ~ListNode();
};

#endif //SYSTEM_PROGRAMMING_1_LISTNODE_H
