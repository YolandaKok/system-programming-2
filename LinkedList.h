//
// Created by yolanda on 27/3/2019.
//

#ifndef SYSTEM_PROGRAMMING_2_LINKEDLIST_H
#define SYSTEM_PROGRAMMING_2_LINKEDLIST_H

#include "ListNode.h"

class LinkedList {
    private:
        ListNode *head;
        int size;
    public:
        LinkedList();
        void add(char *id);
        int find(char *id);
        void print();
        char* getItem(int i);
        int length();
        // Delete
        ~LinkedList();
};

#endif //SYSTEM_PROGRAMMING_2_LINKEDLIST_H