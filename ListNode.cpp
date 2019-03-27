//
// Created by yolanda on 24/2/2019.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ListNode.h"

ListNode::ListNode() {
    this->id = NULL;
}

ListNode::ListNode(char* id) {
    //this->coin = coin;
    this->id = (char*)malloc(strlen(id) + 1);
    strcpy(this->id, id);
    this->next = NULL;
}

void ListNode::insert(char* id, ListNode *head) {
    if(head->id != NULL) {
        ListNode *current = head;
        while(current->next != NULL) {
            current = current->next;
        }
        current->next = new ListNode(id);
    }
    else {
        head->id = (char*)malloc(strlen(id) + 1);
        strcpy(head->id, id);
        head->next = NULL;
    }
}

int ListNode::find(char *id, ListNode *head) {
    if(head->id == NULL)
        return 0;
    int found = 0;
    /* Add to the wallet */
    ListNode *current = head;
    while( current != NULL ) {
        /* If you find the coin just + into the amount else add a new node to the list */
        if(strcmp(current->id, id) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void ListNode::print() {
    ListNode *current = this;
    while( current != NULL ) {
        printf("Client Id: %s \n", current->id);
        current = current->next;
    }
}

ListNode* ListNode::getNext() {
    return this->next;
}

ListNode::~ListNode() {
    free(this->id);
}