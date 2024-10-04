#!/bin/bash

function critical_section() {
    local lockfile="/tmp/test.lock"
    local lockfd=10
    touch "$lockfile"
    eval "exec ${lockfd}<${lockfile}"
    flock -n ${lockfd} || { echo "$$ Failed to acquire lock!"; return 1; }
    echo "$$ processing..."
    sleep 5
    echo "$$ done"
    return 0
}
export -f critical_section

echo "main $$ start"
critical_section &
critical_section &
# sleep 12
wait
pkill -9 -e -P $$  # NOTE 子进程不能正常关闭文件被杀死可能导致后续无法获取锁,不过好像OS知道进程被杀而关闭其占有的文件
echo "main $$ end"

