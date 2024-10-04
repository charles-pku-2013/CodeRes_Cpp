#!/bin/bash

flag=1
flag2=0
# ((flag)) && echo "true" || echo "false"
# Multiple boolean switch in shell script
((flag && flag2)) && echo "true" || echo "false"
((flag && !flag2)) && echo "true" || echo "false"

# toggle switch
# flag=$((1-flag))
# ((flag ^= 1))
((flag = !flag))

if ((flag)); then
    echo "true"
else
    echo "false"
fi

# comparision
# NOTE! only for integer
# [ $1 -ge $2 ] && echo "yes" || echo "no"

# bash /tmp/test.sh 3.14 2.0
# right way for float compare condition
(($(bc -e "$1 >= $2"))) && echo "yes" || echo "no"
