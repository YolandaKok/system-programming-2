//
// Created by yolanda on 27/3/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "LinkedList.h"

using namespace std;

int main(int argc, char *argv[]) {

    LinkedList *list = new LinkedList();
    list->add("3");
    list->add("5");
    list->add("8");
    list->print();
    cout << list->find("13") << endl;
    delete list;
    return 1;
}