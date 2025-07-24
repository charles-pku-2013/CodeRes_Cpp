#!/bin/bash

for i in `seq 20`; do
    curl http://127.0.0.1:8000/api/translate -d '{"text" : "Hello world!"}' &
done

wait

