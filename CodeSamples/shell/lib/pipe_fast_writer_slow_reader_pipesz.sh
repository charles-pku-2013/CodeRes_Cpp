# seems 64k default pipe size. 2250+ items

trap 'echo "pipe error!"; exit 1' SIGPIPE

# ctrl-c 只会终止主进程
while IFS='' read -r line; do
    echo "$line"
    sleep 10
# done < <(pipesz -s1M; for i in `seq 20000`; do
done < <(for i in `seq 20000`; do
        echo "$i" >&2
        # echo "$(date)"
        # timeout -k 5s 5s bash -c 'echo "$(date)"' || exit 101  # timeout退出不会产生SIGPIPE
        timeout -k 5s 5s bash -c 'echo "$(date)"' || kill -- -$$
    done)
