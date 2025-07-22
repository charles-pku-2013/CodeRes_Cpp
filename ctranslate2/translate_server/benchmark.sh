#!/bin/bash

for i in `seq 20`; do
    curl 127.0.0.1:8000/api/translate -d 'hello world' &
done

wait

