#!/bin/bash
# set -x

# for i in $(seq 10); do touch "test ${i}.dat"; done

function process_single_file() {
    local file="$1"
    [ ! -f "$file" ] && { echo "$file not exist!"; return 1; }
    if grep -q '5.dat' <<< "$file"; then
        echo "Error on processing $file"
        return 1
    fi
    echo "Successfully processed $file"
    return 0
}

function process_all() {
    local OIFS="$IFS"
    # trap "break" ERR  # OK
    trap 'IFS="$OIFS"' RETURN  # NOTE!!! trap后命令序列最好用单引号,这里如果换成双引号IFS会变成空
    trap 'trap - ERR; return 1' ERR  # OK don't forget restoring
    flist=$(find * -mindepth 0 -maxdepth 0 -type f -iname '*.dat')
    # NOTE! 需要处理含空格的文件名必须设置IFS
    IFS=$'\n'  # NOTE!!! 转义字符前必须有$
    md5sum <<< "$IFS"
    for file in ${flist}; do
        process_single_file "$file"
    done
    return 0
}

md5sum <<< "$IFS"
process_all
md5sum <<< "$IFS"

#!/bin/bash
function test_fn() {
    trap 'echo "error!"; trap - ERR' ERR  # NOTE!!! restore it if you only want it take effect in function
    # NOTE 以下不会触发trap ERR 因为已经用 && || 处理了，整个语句返回正确
    # ls /tmp1 && echo "success" || echo "fail"
    ls /tmp1
}
test_fn
ls /tmp2

#!/bin/bash
# function err_report() {
    # print lineno in shell script
    # echo "errexit on line $(caller)" >&2
# }
# trap err_report ERR
# echo "hello" | grep 'foo'
# echo "Test End"
