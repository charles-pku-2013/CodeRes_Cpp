#!/bin/bash

# Test: sync_dir.sh src dst -n

PREFIX="/share/sysroot"
config_file="$PREFIX/etc/config"

. $PREFIX/usr/lib/log.sh
. $PREFIX/usr/lib/get_config.sh
. $PREFIX/usr/lib/kill_children.sh

# NOTE read and check args from config file
export ssh_key=$(get_config "ssh_key") && [ -z "$ssh_key" ] && { LOG "Fail to get ssh_key"; exit 1; }
export nas_url=$(get_config "nas_url") && [ -z "$nas_url" ] && { LOG "Fail to get nas_url"; exit 1; }
export port=$(get_config "nas_port") && [ -z "$port" ] && { LOG "Fail to get nas_port"; exit 1; }
[ ! -f "$ssh_key" ] && { LOG "ssh key file ${ssh_key} does not exist!"; exit 1; }

src="$1"
dst="$2"
[ -z "$src" ] && { LOG "Src dir is empty!"; exit 1; }
[ ! -d "$dst" ] && { LOG "Dst dir ${dst} is not a valid directory!"; exit 1; }
shift
shift

dryrun="$1"

set -x
rsync -avi $dryrun --delete -e "ssh -q -p $port -i $ssh_key -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null"  --backup --backup-dir=._rsync_backup_$(date +%Y%m%d_%H%M%S) --no-owner --no-group --no-perms --exclude={".*",".*/","@*/","#*/"} "root@${nas_url}:${src}/" "${dst}/"
set +x
