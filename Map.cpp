#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Map.h"
#include "MapNode.h"

Map::Map() {
    this->size = 0;
    this->head = new MapNode();
}

void Map::add(pid_t key, char *client_id) {
    this->head->add(key, client_id, this->head);
    this->size += 1;
}

char* Map::find(pid_t key) {
    return this->head->find(key, this->head);
}

Map::~Map() {
    MapNode *current = this->head, *temp;
    while(current != NULL) {
        temp = current->getNext();
        delete current;
        current = temp;
    }
}
