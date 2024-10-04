#!/bin/bash

function caculate() {
    echo "$@" | bc -q
}

filelist=()
function get_file_list() {
    OIFS=$IFS
    trap 'IFS=${OIFS}' RETURN
    IFS=$'\r\n'
    filelist=($(find * -type f \( -iname '*.mp4' -o -iname '*.mov' \) -not -iname '*_1080p*'))
}
get_file_list

rm -fv /tmp/task.txt
for i in "${filelist[@]}"; do
    # shell script get file stem main name and extension
    # keep orig extension
    # new_name="${i%.*}_1080p.${i##*.}"
    # fore use mp4
    new_name="${i%.*}_1080p.mp4"

    [ -f "$new_name" ] && continue  # skip already exist
    echo "Registering ${i} ..."

    filter_args=()
    width=$(mediainfo --Inform="Video;%Width%" "$i")
    if (($(caculate "$width >= 2160"))); then
        filter_args+=("scale=iw/2:ih/2")
    fi
    fps=$(mediainfo --Inform="General;%FrameRate%" "$i")
    if (($(caculate "$fps > 40"))); then
        filter_args+=("fps=30")
    fi
    if [ ${#filter_args[@]} -gt 0 ]; then
        # print array with custom delimiter seperator
        vf_arg=$(IFS=','; echo "${filter_args[*]}")
        vf_arg="-vf ${vf_arg}"
    fi

    # orig_bitrate=$(mediainfo --Inform="Video;%BitRate%" "$i")
    # new_bitrate=$(caculate "$orig_bitrate / 1000 * 0.7")
    # buf_size=$(caculate "$new_bitrate * 1.5")
    # float to int 去掉小数点 NOTE 需要加上单位k
    # new_bitrate=${new_bitrate%.*}
    # buf_size=${buf_size%.*}

    # NOTE 用单引号 输出文本也有引号
    # NOTE for audio re-encoding: -c:a libfdk_aac -b:a 256k
    # Linux with NVIDIA CUDA
    # echo "ffmpeg -nostdin -y -hwaccel cuda -i '${i}' $vf_arg -c:v hevc_nvenc -preset p5 -level 5.1 -tier high -b:v 6M -maxrate 9M -bufsize 9M -c:a copy -map_metadata 0 -tag:v hvc1 '${new_name}' > /dev/null 2>&1" >> /tmp/task.txt
    # MacOS
    echo "ffmpeg -nostdin -y -i '${i}' $vf_arg -c:v hevc_videotoolbox -b:v 6M -maxrate 9M -bufsize 9M -c:a copy -map_metadata 0 -tag:v hvc1 '${new_name}' > /dev/null 2>&1" >> /tmp/task.txt
    # MacOS for apple lossless music
    # echo "ffmpeg -nostdin -y -i '${i}' $vf_arg -c:v hevc_videotoolbox -b:v 6M -maxrate 9M -bufsize 9M -c:a alac -map_metadata 0 -tag:v hvc1 '${new_name}' > /dev/null 2>&1" >> /tmp/task.txt
done

# run with parallel NOTE set num of jobs: macos 6, linux 3
parallel -j 6 --bar --joblog /tmp/task.log --retries 5 < /tmp/task.txt && echo "SUCCESS"

# monitor task status
# tail -n 500 -f /tmp/task.log | cut -f1-8
# Or
# less /tmp/task.log
# Shift - f for tailing, In normal mode (not tailing) -S <Enter> toggle wrapping

# Move original raw videos to specific dir 扩展名不要求一致
# find . -type f -name '*_1080p*' -execdir bash -c 'rawfile=$(sed "s/_1080p//" <<< "{}"); [ -f "${rawfile%.*}".* ] && mkdir -p RawVideos && mv -v "${rawfile%.*}".* RawVideos/' \;

# check results
# cat /tmp/task.log | awk '{print $7}' | grep '0' | wc -l
# find . -type f -iname '*_1080p.mp4' | parallel --halt now,fail=1 --joblog /tmp/task.log ffprobe {}

# correct date time (如果命令中有引号等特殊字符必须加-q)
# find . -type f -iname '*_1080p.mp4' | parallel -q --halt now,fail=1 --joblog /tmp/task.log exiftool -v "-FileModifyDate<CreateDate" {}

# On MacOS
## Re-encode from Apple ProRes
# NOTE q:v 过大可能导致过高码率
# ffmpeg -i DJI_0228.mov -c:v hevc_videotoolbox -b:v 100M -c:a copy -map_metadata 0 -tag:v hvc1 DJI_0228.MP4
# ffmpeg -i DJI_0228.mov -vf scale=1920:1080 -c:v hevc_videotoolbox -b:v 8M -maxrate 11M -bufsize 11M -c:a copy -map_metadata 0 -tag:v hvc1 DJI_0228_1080p.MP4
# 也可以只指定 -q:v 让系统自动决定码率 常高于预期
# ffmpeg -i DJI_0228.mov -vf scale=1920:1080 -c:v hevc_videotoolbox -q:v 70 -c:a copy -map_metadata 0 -tag:v hvc1 DJI_0228_1080p.MP4

