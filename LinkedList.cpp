//
// Created by yolanda on 27/3/2019.
//

#include "ListNode.h"
#include "LinkedList.h"

LinkedList::LinkedList() {
    this->head = new ListNode();
    this->size = 0;
}

void LinkedList::add(char *id) {
    this->head->insert(id, this->head);
    this->size += 1;
}

int LinkedList::find(char *id) {
    return this->head->find(id, this->head);
}

void LinkedList::print() {
    this->head->print();
}

char* LinkedList::getItem(int i) {
    return this->head->getItem(i, this->head);
}

int LinkedList::length() {
    return this->size;
}

LinkedList::~LinkedList() {
    ListNode *current = this->head, *temp;
    while(current != NULL) {
        temp = current->getNext();
        delete current;
        current = temp;
    }
}