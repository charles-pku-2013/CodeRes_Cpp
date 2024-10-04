#!/bin/bash

function sig_handler() {
    echo "$$ got the signal"
}
trap sig_handler SIGUSR1

echo "$$ running"
while true; do
    inotifywait -m -r ./
    echo $?   # will return error on signal interruption
done

# Must send to signal group
# kill -USR1 -1133383
