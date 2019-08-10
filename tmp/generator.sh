#!/usr/bin/env bash

i=0
cat tab.h | while read line; do
    i=$((i+1))
    name=$(echo $line | awk '{print $2}' | sed "s/KEY_//g")
    number=$(echo $line | awk '{print $2}')
    printf "    {\"%s\", %s}," $name $number
    if [[ $i = 2 ]]
    then
        i=0
        printf "\n"
    fi
done