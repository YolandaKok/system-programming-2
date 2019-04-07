//
// Created by yolanda on 6/4/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "MapNode.h"

MapNode::MapNode() {
    this->client = NULL;
}

MapNode::MapNode(pid_t key, char *client_id) {
    this->client = (char*)malloc(strlen(client_id) + 1);
    strcpy(this->client, client_id);
    this->key = key;
    this->next = NULL;
}

void MapNode::add(pid_t key, char *client_id, MapNode *head) {
    if(head->client != NULL) {
        MapNode *current = head;
        while(current->next != NULL) {
            current = current->next;
        }
        current->next = new MapNode(key, client_id);
    }
    else {
        head->client = (char*)malloc(strlen(client_id) + 1);
        strcpy(head->client, client_id);
        head->key = key;
        head->next = NULL;
    }
}

char* MapNode::find(pid_t key, MapNode *head) {
    /* Add to the wallet */
    MapNode *current = head;
    while( current != NULL ) {
        /* If you find the coin just + into the amount else add a new node to the list */
        if(current->key == key) {
            return current->client;
        }
        current = current->next;
    }
}

MapNode* MapNode::getNext() {
    return this->next;
}

MapNode::~MapNode() {
    free(this->client);
}
