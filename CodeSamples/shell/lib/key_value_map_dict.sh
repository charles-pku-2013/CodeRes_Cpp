#!/bin/sh
# map dict example in shell script
# https://fedingo.com/how-to-use-key-value-dictionary-in-shell-script/

echo "Declare Dictionary ..."
declare -A test_dict
# export test_dict  # NOTE cannot be exported

echo "Add Key-Value Pairs in Dictionary ..."
test_dict[key1]=value1
test_dict['key2']='value2'
key_var='key3'
value_var='value3'
test_dict[$key_var]=$value_var

echo "Retrieve Value from Dictionary ..."
echo ${test_dict[key1]}
echo ${test_dict[key2]}

# echo ${test_dict}      # empty string
# echo ${test_dict[@]}	 # only values

echo "Update Existing Key-Value in Dictionary ..."
test_dict[key1]='new_value1'

echo "Check if key exists in dictionary ..."
test_key="key_var"
test -v test_dict["$test_key"] && echo "${test_key} exists!" || echo "${test_key} does not exist!"

echo "Iterate over a dictionary ..."
for key in "${!test_dict[@]}"; do
    echo "$key ${test_dict[$key]}"
    export ${key}=${test_dict[$key]}  # This OK
done

function process() {
    # NOTE doesn't work
    # echo "Iterate over a dictionary ..."
    # for key in "${!test_dict[@]}"; do
        # echo "$key ${test_dict[$key]}"
    # done
    echo "$key3"
}
export -f process
find . -type f -exec bash -c 'process {}' \;

echo "Remove key-value pair from dictionary"
unset test_dict[key1]
unset test_dict['key2']
unset test_dict[$key_var]
echo ${test_dict}
