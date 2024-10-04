#!/system/bin/sh

# required: $pidfile
function kill_old_process() {
    test -f "$pidfile" || return
    local pid=$(cat "$pidfile")
    test -z "$pid" -o $pid -le 1 && return
    local pgid=$(ps -o pgid= -p "$pid")
    [ "$pid" == "$pgid" ] && pid="-$pid"
    kill -- "$pid" 2>/dev/null || return   # pidfile exists but process not
    for i in `seq 10`; do
        sleep 1
        test -f "$pidfile" || return
        kill -- "$pid" 2>/dev/null || return
    done
    sleep 1
    kill -9 "$pid" 2>/dev/null
}
export -f kill_old_process
