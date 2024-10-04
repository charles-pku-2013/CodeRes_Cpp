#!/bin/bash
# set -x

# sudo add-apt-repository -y ppa:flacon
# sudo apt-get update
# sudo apt-get install -y shntool flacon
# NOTE ape not supported on macos, to check `shntool -a`
# brew install shntool flacon

# Usage:
# find . -not -path '*/.*' -not -path '*@Recycle*' -type f -iname '*.cue' | parallel -u "cd {//} && cue_split_flac.sh -K {/}"

keep_original=1

POSITIONAL_ARGS=()
while [[ $# -gt 0 ]]; do
  case $1 in
      -K|--no-keep-original)
          keep_original=0
          shift
          ;;
      -*|--*)
          echo "Unknown option $1"
          exit 1
          ;;
      *)
          POSITIONAL_ARGS+=("$1") # save positional arg
          shift # past argument
          ;;
  esac
done
set -- "${POSITIONAL_ARGS[@]}" # restore positional parameters

cueFile="$1"
test -z "$cueFile" && { echo "Null cue file!"; exit -1; }
baseDir="$(dirname "$cueFile")"

# NOTE!!! 文件名里包含空格需要用双引号包裹变量 handling spaces in filename in shell script
n_audio_file=`grep '^FILE' "$cueFile" | wc -l`
test ${n_audio_file} -gt 1 && \
        { echo "WARNING Skipping ${cueFile} which has more than one audio file"; exit -1; }

# get corresponding audio file
audioFile="`grep '^FILE' "$cueFile" | awk -F'"' '{print $2}'`"
test -z "$audioFile" && { echo "ERROR No audio file config found in ${cueFile}"; exit -1; }
audioFile="${baseDir}/${audioFile}"
test ! -f "$audioFile" && { echo "ERROR ${cueFile} whose audio file ${audioFile} does not exist!"; exit -1; }

tmpDir="tmp_$(md5sum <<< "$cueFile" | awk '{print $1}')"
mkdir -p ${tmpDir} && rm -rf ${tmpDir}/* || { echo "Cannot create work dir for ${cueFile}"; exit -1; }
trap "rm -rf ${tmpDir}; trap - EXIT; exit" EXIT SIGINT SIGHUP SIGQUIT SIGABRT SIGTERM SIGSTOP SIGPIPE

echo "Processing ${cueFile} ..."
shnsplit -O always -d "$tmpDir" -f "$cueFile" -o "flac flac --best --verify -o %f -" -t "%n - %t" "$audioFile" || \
    {  echo "ERROR shnsplit fail for ${cueFile}"; exit -1;  }
# Remove pregap
find ${tmpDir} -iname '*pregap.flac' -delete
# tag the flac files based on cue
# On macos default is cuetag.sh
# Wildcard is guaranteed to be sorted
# ls *-*.flac | sort | tr '\n' '\0' | xargs -0  cuetag CDImage.cue
cuetag "$cueFile" ${tmpDir}/*.flac && mv $tmpDir/*.flac ./ || { echo "ERROR cuetag fail for ${cueFile}"; exit -1; }

test ${keep_original} -eq 0 && { rm -f "$cueFile"; rm -f "$audioFile"; }

