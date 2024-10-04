#!/data/data/com.termux/files/usr/bin/bash
# set -x

backup=0

POSITIONAL_ARGS=()
while [[ $# -gt 0 ]]; do
  case $1 in
    -b)
        backup=1
        shift
        ;;
    -h|--help)
        echo "Usage: videoomerge [-b] files"
        exit 0
        ;;
    -*)
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

if [ $# -eq 0 ]; then
    echo "No input file provided!"
    exit -1
fi

filelist=()
function get_file_list() {
    OIFS=$IFS
    trap 'IFS=${OIFS}' RETURN
    IFS=$'\r\n'
    filelist=($(ls "$@" | sort | uniq))
}
get_file_list "$@"   # NOTE 处理含空格文件名必须用引号 pass arguments with spaces

# check array is empty
[ ${#filelist[@]} -eq 0 ]  && { echo "No input file provided!"; exit -1; }

first_file="${filelist[0]}"
base_first_file="$(basename "$first_file")"
out_file="merge_${base_first_file}"
mkv_file="${out_file%.*}.mkv"
backup_dir="_merge_bak_${base_first_file%.*}"

# DEBUG
# echo ${#filelist[@]}
# echo "${filelist[@]}"
# echo "$first_file"
# echo "$out_file"
# echo "$mkv_file"
# echo "$backup_dir"
# exit 0

# NOTE quotes required if containing spaces
# ls "${filelist[@]}"
# exit 0

# conditional string assignment
tagv=$(mediainfo --Inform="Video;%CodecID%" "$first_file" 2>/dev/null || echo "")
if [ ! -z "$tagv" ]; then
    tagv="-tag:v ${tagv}"
fi

echo "Creating ${mkv_file} from ${filelist[@]} ..."
mkvmerge -o "$mkv_file" [ "${filelist[@]}" ] ||
            { echo "Failed to create mkv ${mkv_file}"; exit -1; }
echo "Converting ${mkv_file} to ${out_file} ..."
ffmpeg -y -i "$mkv_file" -i "$first_file" -c copy ${tagv} -map_metadata 1 "$out_file" ||
            { echo "Failed to create out file ${out_file}"; exit -1; }
# copy file modify date time from first file
# exiftool -TagsFromFile "$first_file" "-all:all>all:all" "$out_file"    # TODO
exiftool -TagsFromFile "$first_file" -FileCreateDate -FileModifyDate "$out_file"
# backup outfile
mv -v "$out_file" "${out_file}.bak"
echo "Removing mkv file..."
rm -fv "$mkv_file"
echo "Backuping input files..."
mkdir -p "$backup_dir"
mv -fv "${filelist[@]}" "${backup_dir}/"
# restore outfile
mv -v "${out_file}.bak" "${first_file}"
if ((!backup)); then
    echo "Removing backup..."
    rm -rfv "$backup_dir"
fi
echo "Success!"
