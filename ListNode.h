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
        int regular;
    public:
        ListNode();
        ListNode(char* id);
        ListNode(char* id, int regular);
        void insert(char* id, ListNode *head);
        void insert(char* id, ListNode *head, int regular);
        ListNode* getNext();
        void print();
        int find(char *id, ListNode *head);
        ListNode* getItemNode(int i, ListNode *head);
        int getRegular();
        void deleteItem(char *item, ListNode **head);
        char* getItem(int i, ListNode *head);
        ~ListNode();
};

#endif //SYSTEM_PROGRAMMING_1_LISTNODE_H
