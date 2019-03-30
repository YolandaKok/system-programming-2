#!/bin/bash

count=0
bytes_send=0
two=2
while read line; do
    for word in ${line}; do
        echo "word = '$word'"
        count=$((count+1))
        if [ ${count} -eq ${two} ]; then
            bytes_send=$((bytes_send+$word))
        fi
    done
    count=0
done

echo ${bytes_send}