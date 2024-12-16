#!/bin/bash

# variables provided by dockerfile

set -x

export GIT_SSH_COMMAND="ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no"

# set gcc7 environment
source /opt/rh/devtoolset-7/enable
export CC=$(which gcc)
export CXX=$(which g++)
export AR=$(which ar)
export NM=$(which nm)
export LD=$(which ld)
gcc --version

# BAZEL="/usr/bin/bazelisk"
BAZEL="/usr/bin/bazel"

test -z "$app_git_url" && { echo "app_git_url not specified!"; exit -1; }
test -z "$app_build_target" && { echo "app_build_target not specified!"; exit -1; }
test -z "$app_deploy_dir" && { echo "app_deploy_dir not specified!"; exit -1; }

app_name=`(awk -F'/' '{print $NF}' | sed 's/\.git$//') <<< "$app_git_url"`
test -z "$app_name" && { echo "Failed to get app_name!"; exit -1; }

mkdir -p /repo && rm -rf /repo/* && cd /repo

# get app code and cd into workspace dir if specified
printf "Fetching app code from %s ...\n" "$app_git_url"
git clone "$app_git_url" "$app_name" || { echo "git clone app code fail!"; exit -1; }
cd "$app_name"
if [ ! -z "$app_git_tag" ]; then
    git checkout "$app_git_tag" || { echo "Invalid app git tag ${app_git_tag}"; exit -1; }
fi

# get redstone server git url, name = "redstone" 第一次出现的位置，后面紧跟remote和commit，不能有整行注释
server_git_url=`cat "$redstone_repo_file" | grep -A3 -E '^\s*name\s*=\s*"\<redstone\>"' | grep -E '^\s*\<remote\>\s+=' | awk -F'"' '{print $(NF-1)}'`
orig_server_commitid=`cat "$redstone_repo_file" | grep -A3 -E '^\s*name\s*=\s*"\<redstone\>"' | grep -E 'commit.*=' | awk '{print $NF}' | tr -d '\",'`

# Replace redstone_server repo commitid with server_commitid
if [ ! -z "$server_commitid" ]; then
    commit_lineno=`cat "$redstone_repo_file" | grep -n -A3 -E '^\s*name\s*=\s*"\<redstone\>"' | grep -E '^[0-9]+-\s+\<commit\>' | awk -F'-' '{print $1}'`
    test -z "$commit_lineno" && { echo "Failed to get server repo commit id!"; exit -1; }
    echo "Setting server repo commitid to ${server_commitid} ..."
    sed -i "${commit_lineno}s/\".*\"/\"${server_commitid}\"/" "$redstone_repo_file"
else
    server_commitid="$orig_server_commitid"
fi

# build server
case $build_server in
    yes|Yes|y|Y)
        test -z "$server_git_url" && { echo "Failed to get server_git_url!"; exit -1; }
        printf "Fetching redstone server code from %s ...\n" "$server_git_url"
        cd /repo && git clone "$server_git_url" redstone && cd redstone || { echo "git clone redstone server fail!"; exit -1; }
        if [ ! -z "$server_commitid" ]; then
            # 切换到指定的server_commitid
            git checkout "$server_commitid" || { echo "Invalid redstone server commit id ${server_commitid}"; exit -1; }
        fi
        find . -name '.bazelrc' -exec bash -c 'echo "build --distdir=/bazel_distdir" >> "{}"' \;
        echo "Building redstone server ${server_commitid} ..."
        $BAZEL build ${build_options} //redstone:redstone_server || { echo "Failed to build redstone_server!"; exit -1; }
        mkdir -p ${REDSTONE_PATH}/bin ${REDSTONE_PATH}/lib && rm -rf ${REDSTONE_PATH}/bin/redstone_server ${REDSTONE_PATH}/lib/*
        deplist=$(ldd bazel-bin/redstone/redstone_server | grep bazel-bin | awk '{if (match($3,"/")){ print $3}}')
        cp -L -n -v $deplist ${REDSTONE_PATH}/lib/
        cp bazel-bin/redstone/redstone_server ${REDSTONE_PATH}/bin/
        $BAZEL clean
        ;;
    *)
        ;;
esac

echo "Building app ..."
cd /repo/"$app_name"
if [ ! -z "$app_workspace" ]; then
    cd "$app_workspace" || { echo "App workspace dir ${app_workspace} does not exist!"; exit -1; }
fi
find . -name '.bazelrc' -exec bash -c 'echo "build --distdir=/bazel_distdir" >> "{}"' \;
$BAZEL build ${build_options} "$app_build_target" || { echo "Failed to build app!"; exit -1; }

echo "Deploying app ..."
# 配置文件保存在app_workspace下面的server_config目录
mkdir -p ${app_deploy_dir} && rm -rf ${app_deploy_dir}/*  \
    && cp -a server_config/* ${app_deploy_dir}/ \
    && rm -rf ${app_deploy_dir}/libs/*
app_so=$(sed 's/^\/\/://' <<< "$app_build_target")
cp -f bazel-bin/"$app_so" "$app_deploy_dir"/libs/
echo "$app_so" > "$app_deploy_dir"/libs/libs.txt
deplist=$(ldd "bazel-bin/${app_so}" | grep bazel-bin | awk '{if (match($3,"/")){ print $3}}')
cp -L -n -v $deplist ${app_deploy_dir}/libs/
patchelf --add-rpath '$ORIGIN/.' "$app_deploy_dir"/libs/"$app_so"

$BAZEL clean --expunge
rm -rf /repo
