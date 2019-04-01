#!/bin/bash

count=0

bytes_sent=0
flag1=0

bytes_received=0
flag=0

flag2=0
current_id=-1

num_of_sent=0
num_of_received=0

count_exit=0

while read line; do
    # if it is the first line
    for word in ${line}; do
        #echo "word = '$word'"
        if [[ ${word} == "Client" ]]; then
            flag2=1
        fi

        if [[ ${flag2} -eq "1" ]] && [[ ${count} -eq "1" ]]; then
            flag2=0
            if [[ ${word} -gt ${current_id} ]]; then
                current_id=${word}
            fi
        fi

        if [[ ${word} == "Received" ]]; then
            flag=1
        fi

        if [[ ${flag} -eq "1" ]] && [[ ${count} -eq "2" ]]; then
            flag=0
            bytes_received=$((bytes_received+word))
            num_of_received=$((num_of_received+1))
        fi

        if [[ ${word} == "Sent" ]]; then
            flag1=1
        fi

        if [[ ${flag1} -eq "1" ]] && [[ ${count} -eq "2" ]]; then
            flag1=0
            bytes_sent=$((bytes_sent+word))
            num_of_sent=$((num_of_sent+1))
        fi

        if [[ ${word} == "Exit" ]]; then
            count_exit=$((count_exit+1))
        fi

        count=$((count+1))
    done
    count=0
done

echo "Greatest id: ${current_id}"
echo "Total bytes sent: ${bytes_sent}"
echo "Total bytes received: ${bytes_received}"
echo "Number of files sent: ${num_of_sent}"
echo "Number of files received: ${num_of_received}"
echo "Number of clients that have exited: ${count_exit}"