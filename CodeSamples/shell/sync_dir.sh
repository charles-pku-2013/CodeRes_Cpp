#!/data/data/com.termux/files/usr/bin/bash
# set -x

config_file="/sdcard/usr/etc/config"
export remote_path=""
pidfile="$PREFIX/var/run/sync_dir_$(md5sum <<< $PWD | awk '{print $1}').pid"

. /sdcard/usr/lib/log.sh
. /sdcard/usr/lib/get_config.sh
. /sdcard/usr/lib/kill_old_process.sh

kill_old_process
echo "$$" > $pidfile
LOG "sync_dir for ${PWD} $$ started."

function terminate() {
    rm -f $pidfile
    pkill -9 -P $$
    LOG "sync_dir $$ terminated."
}
trap "terminate; trap - EXIT; exit" EXIT SIGINT SIGHUP SIGQUIT SIGABRT SIGTERM SIGSTOP SIGPIPE

export ssh_key=$(get_config "ssh_key") && [ -z "$ssh_key" ] && { LOG "Fail to get ssh_key"; exit 1; }
export nas_url=$(get_config "nas_url") && [ -z "$nas_url" ] && { LOG "Fail to get nas_url"; exit 1; }
export port=$(get_config "nas_port") && [ -z "$port" ] && { LOG "Fail to get nas_port"; exit 1; }
[ ! -f "$ssh_key" ] && { LOG "ssh key file ${ssh_key} does not exist!"; exit 1; }

# parse args
POSITIONAL_ARGS=()
while [[ $# -gt 0 ]]; do
    case $1 in
        -rpath)
            remote_path=$2
            shift
            shift
            ;;
        *)
            POSITIONAL_ARGS+=("$1") # save positional arg
            shift # past argument
            ;;
    esac
done
set -- "${POSITIONAL_ARGS[@]}" # restore positional parameters

# read "remote_path" config, this will override command line arg
function read_config_file() {
    local config_file=".sync/config"
    [ ! -f "$config_file" ] && return
    local rpath=$(get_config "rpath" "$config_file")
    [ ! -z "$rpath" ] && remote_path="$rpath"
}
read_config_file
[ -z "$remote_path" ] && { LOG "remote_path is not specified!"; exit 1; }

export remote_path="root@${nas_url}:${remote_path}"

function full_sync() {
    LOG "Performing full sync..."
    for i in `seq 10`; do
        rsync -ai --size-only --exclude={".*",".*/"} \
            -e "ssh -q -p ${port} -i ${ssh_key} -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null" ./ "${remote_path}/" && \
                { LOG "Full sync success"; return; } || LOG "Full sync fail, try again..."
        sleep 3
    done
    LOG "WARNING! full sync fail"
}

function single_sync() {
    local filepath="$1"
    LOG "Syncing ${filepath} ..."
    while true; do
        [ ! -e "$filepath" ] && return
        rsync -aiR --size-only --exclude={".*",".*/"} \
            -e "ssh -q -p ${port} -i ${ssh_key} -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null" "$filepath" "${remote_path}/" && \
            { LOG "Successfully sync $filepath"; return; } || LOG "Failed to sync ${filepath}, try again ..."
        sleep 3
    done
}

LOG "sync_dir $$ started at ${PWD} rpath=${remote_path}"

# first, run full dir sync
full_sync

while true; do
    while IFS=':' read event dir file; do
        [ -z "$file" ] && continue
        filepath="${dir}/${file}"
        single_sync "$filepath"
    done < <(inotifywait -q -m -r -e close_write,moved_to --exclude "/\..+" --format "%e:%w:%f" ./)
done
