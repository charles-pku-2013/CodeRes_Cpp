#!/bin/bash

function array_basic() {
    # https://www.geeksforgeeks.org/array-basics-shell-scripting-set-1/
    # To declare static Array
    arr=(prakhar ankit 1 rishabh manish abhinav)

    # To print all elements of array
    echo ${arr[@]}	 # prakhar ankit 1 rishabh manish abhinav
    echo ${arr[*]}	 # prakhar ankit 1 rishabh manish abhinav
    echo ${arr[@]:0} # prakhar ankit 1 rishabh manish abhinav
    echo ${arr[*]:0} # prakhar ankit 1 rishabh manish abhinav

    # To print first element
    echo ${arr[0]}	 # prakhar
    echo ${arr}		 # prakhar

    # To print particular element
    echo ${arr[3]}	 # rishabh
    echo ${arr[1]}	 # ankit

    # To print elements from a particular index
    echo ${arr[@]:0} # prakhar ankit 1 rishabh manish abhinav
    echo ${arr[@]:1} # ankit 1 rishabh manish abhinav
    echo ${arr[@]:2} # 1 rishabh manish abhinav
    echo ${arr[0]:1} # rakhar

    # To print elements in range
    echo ${arr[@]:1:4} # ankit 1 rishabh manish
    echo ${arr[@]:2:3} # 1 rishabh manish
    echo ${arr[0]:1:3} # rak

    # Length of Particular element
    echo ${#arr[0]}	 # 7
    echo ${#arr}	 # 7

    # Size/Length of an Array
    echo ${#arr[@]}	 # 6
    echo ${#arr[*]}	 # 6

    # Search in Array
    echo ${arr[@]/*[aA]*/} # 1

    # Replacing Substring Temporary
    echo ${arr[@]//a/A}	 # prAkhAr Ankit 1 rishAbh mAnish AbhinAv
    echo ${arr[@]}		 # prakhar ankit 1 rishabh manish abhinav
    echo ${arr[0]//r/R}	 # pRakhaR
}

function append_to_array() {
    arr=()
    arr+=("foo")  # 数组元素必须用括号
    arr+=("bar")
    arr+=(1)
    arr+=($CC)
    echo ${arr[*]}
    echo ${#arr[*]}
    arr=()
    echo ${arr[*]}
    echo ${#arr[*]}
}

function build_array_from_string() {
    str="foo,bar,beijing,shanghai,shenzhen"
    IFS=',' read -r -a arr <<< "$str"
    echo ${arr[@]}
    # iterate over array
    for item in ${arr[@]}; do
        echo $item
    done
}

function array_as_command_arg() {
    str="/tmp/a 1,/tmp/a 2"
    IFS=',' read -r -a arr <<< "$str"
    echo ${arr[@]}
    inotifywait -q -m "${arr[@]}"
}

function print_array_with_delimiter() {
    foo=(a "b c" d)
    bar=$(IFS=','; echo "${foo[*]}")  # 必须用*
    echo "$bar"
}

# array_basic
# append_to_array
# build_array_from_string
print_array_with_delimiter
