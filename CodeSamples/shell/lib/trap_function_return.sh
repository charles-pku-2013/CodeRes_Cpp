#!/bin/bash
# set -x

function test_trap() {
    trap "echo -e 'function terminating'" RETURN
    echo "function running"
}

test_trap

echo "Test End"
