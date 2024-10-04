#!/bin/bash

# trim string / file with std
# sed -e 's/^[[:space:]]*//' | sed -e 's/[[:space:]]*$//'

str="  value1:val ue2:val3  "

IFS=':' read v1 v2 v3 <<< "$str"
echo "$v1"
echo "$v2"
echo "$v3"
# get string length strlen
echo "${#v3}"

# trim string with xargs
v1=$(echo "$v1" | xargs)
v2=$(echo "$v2" | xargs)
v3=$(echo "$v3" | xargs)
echo "$v1"
echo "$v2"
echo "$v3"
echo "${#v3}"
