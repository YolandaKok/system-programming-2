#!/bin/bash

count=0
bytes_send=0
bytes_received=0
flag=0
two=2
while read line; do
    # if it is the first line
    for word in ${line}; do
        echo "word = '$word'"
        if [[ ${word} == "Received" ]]; then
            flag=1
        fi
        if [[ ${flag} -eq "1" ]] && [[ ${count} -eq "2" ]]; then
            flag=0
            bytes_received=$((bytes_received+word))
            echo ${word}
        fi
        count=$((count+1))
    done
    count=0
done

echo ${bytes_received}

