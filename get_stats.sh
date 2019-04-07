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

num_of_clients=0

declare -a clients_list


while read line; do
    # if it is the first line
    for word in ${line}; do
        if [[ ${word} == "Client" ]]; then
            flag2=1
            num_of_clients=$((num_of_clients+1))
        fi

        if [[ ${flag2} -eq "1" ]] && [[ ${count} -eq "1" ]]; then
            flag2=0
            if [[ ${word} -gt ${current_id} ]]; then
                current_id=${word}
            fi
            if [[ ${num_of_clients} -eq "1" ]]; then
                clients_list[${num_of_clients}-1]=${word}
                minimum_client=${word}
            fi
            if [[ ${num_of_clients} -gt "1" ]]; then
                clients_list[${num_of_clients}-1]=${word}
                if [[ ${minimum_client} -gt ${word} ]]; then
                    minimum_client=${word}
                fi
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

echo "Client Ids: ${clients_list[@]}"
echo "Number of clients which connected to the network is: ${num_of_clients}"
echo "Minimum client id: ${minimum_client}"
echo "Greatest client id: ${current_id}"
echo "Total bytes sent: ${bytes_sent}"
echo "Total bytes received: ${bytes_received}"
echo "Number of files sent: ${num_of_sent}"
echo "Number of files received: ${num_of_received}"
echo "Number of clients that have exited: ${count_exit}"