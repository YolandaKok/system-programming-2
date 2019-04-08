#!/bin/bash

# Check input parameters

# Directory Name
dir_name=$1
# Number of files that we want to create
num_of_files=$2
# Num of Directories
num_of_dirs=$3
# Levels of the directories
levels=$4

# TODO: Check if zero parameters

# Check if num_of_files and num_of_dirs are greater than zero
if [[ "$2" -lt "0" ]] || [[ "$2" -eq "0" ]]; then
    echo Error with number of files
    exit
fi

if [[ "$3" -lt "0" ]]; then
    echo Error with number of directories
    exit
fi

# Check the number of levels
if [[ "$4" -lt "0" ]]; then
    echo Levels have to be greater than zero
    exit
fi

# Check if directory exists

if [[ -d ${dir_name} ]]; then
    echo Directory already exists !
    exit
else
    # If does not exist, create the directory
    mkdir -p ${dir_name};
fi

declare -a Directory_Names


for((i=0;i<num_of_dirs;i++)); do
    num_of_characters=$(( ( RANDOM % 8 )  + 1 ))
    Directory_Names[i]=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w ${num_of_characters} | head -n 1)
done

# Loop the directory names
for((i=0;i<$num_of_dirs;i++)); do
    echo ${Directory_Names[i]}
done

current_level=0
current_directory="$dir_name"

declare -a Directory_Names_New

Directory_Names_New[0]=${current_directory}

# For every directory
for((i=0;i<$num_of_dirs;i++)); do
    if [[ "$current_level" -lt "$levels" ]]; then
        current_directory="$current_directory/${Directory_Names[i]}"
        echo ${current_directory}
        Directory_Names_New[i+1]=${current_directory}
        mkdir -p ${current_directory};
        current_level=$((current_level+1))
    fi
    if [[ "$current_level" -eq "$levels" ]]; then
        current_directory="$dir_name"
        current_level=0
    fi
done

declare -a File_Names

# Create the random file names
for((i=0;i<num_of_files;i++)); do
    num_of_characters=$(( ( RANDOM % 8 )  + 1 ))
    File_Names[i]=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w ${num_of_characters} | head -n 1)
    echo ${File_Names[i]}
done

j=0
current_directory="$dir_name"
# Round Robin to put the files
for((i=0;i<$num_of_files;i++)); do
    path_to_file="${Directory_Names_New[j]}/${File_Names[i]}"
    touch ${path_to_file}
    num_of_characters=$(( (( RANDOM % ((1024 * 128))) + 1024)))
    echo ${num_of_characters}
    cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w ${num_of_characters} | head -n 1 > ${path_to_file}
    echo ${path_to_file}
    j=$((j+1))
    if [[ "$j" -eq "$num_of_dirs+1" ]]; then
        j=0
    fi
done

