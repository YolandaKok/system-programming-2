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
        void add(char *id, int regular);
        int find(char *id);
        ListNode* getItemById(char *id);
        void print();
        char* getItem(int i);
        ListNode* getListNodeItem(int i);
        void update(char *id, int completed);
        void deleteItem(char *item);
        int length();
        // Delete
        ~LinkedList();
};

#endif //SYSTEM_PROGRAMMING_2_LINKEDLIST_H
