#!/bin/bash

# here must use eval
ftype="-name '*.h' -o -name '*.cc'"
eval "find . -type f \( "$ftype" \) -print"

#!/bin/bash
# set -x

sample_file="$1"
model_name="$2"
server="$3"

[ -z "$sample_file" -o -z "$model_name" -o -z "$server" ] && echo "Usage: $0 sample_file server_addr"

# eval and xargs example
for lineno in $(seq $(wc -l "$sample_file" | awk '{print $1}')); do
    cmd="sed -n \"${lineno}p\" "$sample_file" | \
            xargs -I{} curl -X POST -H 'accept: application/json' -d \
            '{\"model_name\":\""$model_name"\",\"data\":\""{}"\"}' \
            ${server}:8003/ServerOpsService/ops?model_verify 2>/dev/null"
    result="$(eval $cmd)"
    if ! grep -q '"status" : 0' <<< "$result"; then
        echo "WRONG on line $lineno !!!"
        echo "$result"
    fi
done
