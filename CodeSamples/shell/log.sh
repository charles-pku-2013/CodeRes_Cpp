#!/system/bin/sh

function LOG() {
    local NOW_DATE_STD=$(date +"%Y-%m-%d %H:%M:%S")
    echo -e "[$NOW_DATE_STD] $@" >&2
}
export -f LOG

