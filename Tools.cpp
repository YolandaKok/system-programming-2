//
// Created by yolanda on 3/4/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include "Tools.h"

int numOfDigits(int number) {
    int count = 0;
    while(number != 0) {
        number = number / 10;
        count++;
    }
    return count;
}