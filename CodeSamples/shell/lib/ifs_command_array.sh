#!/bin/bash

md5sum <<< "$IFS"  # fff8564c076d5a9ebc34aa26b44099f1
OIFS=$IFS

# NOTE read command output into array
IFS=$'\r\n' flist=($(ls $1 | sort))
echo ${flist[@]}
echo ${#flist[@]}  # num of files

# 此时还不能恢复IFS
for file in ${flist[@]}; do
    echo "$file"
done

IFS=$OIFS
md5sum <<< "$IFS"

