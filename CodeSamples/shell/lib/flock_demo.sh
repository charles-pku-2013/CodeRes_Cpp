#!/bin/bash

# flock in cron redirect output
# 0 2 * * * flock -n /tmp/sync_ScPhoto.lock bash -c "/usr/local/sbin/sync_dir.sh /ScPhoto /photo/ScPhoto > /share/sysroot/var/log/sync_ScPhoto.log 2>&1"

set -x

lockfile="/tmp/test.lock"
lockfd=4

# WRONG NOTE!!! flock 跟lock文件路径必须跟命令一起用, like: flock $lockfile sleep 5
# flock -n "$lockfile" cmd || { echo "Failed to acquire lock!"; exit 1; }

touch "$lockfile"  # 必须先手动建立
eval "exec ${lockfd}<${lockfile}" # open 关联 fd; 这里必须用eval
# must use fd in script
flock -n ${lockfd} || { echo "Failed to acquire lock!"; exit 1; }

# NOTE!!! 注意顺序 trap 必须在获取锁之后, 否则获取锁失败退出会关掉fd
trap "" SIGPIPE
# trap内容已经有引号，相当于eval
trap "exec ${lockfd}<&-; rm -f "$lockfile"; trap - EXIT; exit" EXIT SIGINT SIGHUP SIGQUIT SIGABRT SIGTERM SIGSTOP

while true; do
    echo "$$ running..."
    sleep 1
done
# eval "exec ${lockfd}<&-"    # close lockfile and release lock





#!/bin/bash
set -x

lockfile="/tmp/test.lock"

# WRONG NOTE!!! flock 跟lock文件路径必须跟命令一起用, like: flock $lockfile sleep 5
# flock -n "$lockfile" || { echo "Failed to acquire lock!"; exit 1; }

touch "$lockfile"  # 必须先手动建立
exec 4<"$lockfile" # open 关联 fd

# must use fd in script
flock -n 4 || { echo "Failed to acquire lock!"; exit 1; }

trap "" SIGPIPE
trap "exec 4<&-; rm -f "$lockfile"; trap - EXIT; exit" EXIT SIGINT SIGHUP SIGQUIT SIGABRT SIGTERM SIGSTOP

while true; do
    echo "$$ running..."
    sleep 1
done
# exec 4<&-    # close
