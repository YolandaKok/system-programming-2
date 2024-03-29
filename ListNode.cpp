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
    this->tries = 0;
    this->completed = 0;
    this->next = NULL;
}

ListNode::ListNode(char* id, int regular) {
    this->id = (char*)malloc(strlen(id) + 1);
    strcpy(this->id, id);
    this->regular = regular;
    this->tries = 0;
    this->completed = 0;
    this->next = NULL;
}

void ListNode::insert(char* id, ListNode *head, int regular) {
    if(head->id != NULL) {
        ListNode *current = head;
        while(current->next != NULL) {
            current = current->next;
        }
        current->next = new ListNode(id, regular);
    }
    else {
        head->id = (char*)malloc(strlen(id) + 1);
        strcpy(head->id, id);
        head->regular = regular;
        head->tries = 0;
        head->completed = 0;
        head->next = NULL;
    }
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
        head->tries = 0;
        head->completed = 0;
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

ListNode* ListNode::getItemById(char *id, ListNode *head) {
    /* Add to the wallet */
    ListNode *current = head;
    while( current != NULL ) {
        /* If you find the coin just + into the amount else add a new node to the list */
        if(strcmp(current->id, id) == 0) {
            return current;
        }
        current = current->next;
    }
}

void ListNode::update(char *id, ListNode *head, int completed) {
    /* Add to the wallet */
    ListNode *current = head;
    while( current != NULL ) {
        /* If you find the coin just + into the amount else add a new node to the list */
        if(strcmp(current->id, id) == 0) {
            this->completed = completed;
            if(completed != 1)
                this->tries += 1;
        }
        current = current->next;
    }
}

int ListNode::getTries() {
    return this->tries;
}

int ListNode::getIfComplete() {
    return this->completed;
}

char* ListNode::getItem(int i, ListNode *head) {
    ListNode *current = head;
    int count = 0;
    while( current != NULL ) {
        if(count == i)
            return current->id;
        current = current->next;
        count++;
    }
}

void ListNode::deleteItem(char *item, ListNode **head) {
    // Store head node
    ListNode *temp = *head, *prev;

    // If head node itself holds the key to be deleted
    if (temp != NULL && (strcmp(temp->id, item) == 0))
    {
        *head = temp->next;   // Changed head
        delete temp;               // free old head
        return;
    }

    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && (strcmp(temp->id, item) != 0))
    {
        prev = temp;
        temp = temp->next;
    }

    // If key was not present in linked list
    if (temp == NULL) return;

    // Unlink the node from linked list
    prev->next = temp->next;

    delete temp;  // Free memory
}

int ListNode::getRegular() {
    return this->regular;
}

ListNode* ListNode::getItemNode(int i, ListNode *head) {
    ListNode *current = head;
    int count = 0;
    while( current != NULL ) {
        if(count == i)
            return current;
        current = current->next;
        count++;
    }
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