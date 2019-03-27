//
// Created by yolanda on 27/3/2019.
//

#include "ListNode.h"
#include "LinkedList.h"

LinkedList::LinkedList() {
    this->head = new ListNode();
}

void LinkedList::add(char *id) {
    this->head->insert(id, this->head);
}

int LinkedList::find(char *id) {
    return this->head->find(id, this->head);
}

void LinkedList::print() {
    this->head->print();
}

LinkedList::~LinkedList() {
    ListNode *current = this->head, *temp;
    while(current != NULL) {
        temp = current->getNext();
        delete current;
        current = temp;
    }
}