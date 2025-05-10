#!/data/data/com.termux/files/usr/bin/bash
# set -x

raw_types="-name '*.RAF' -o -name '*.DNG' -o -name '*.NEF'"

dryrun=1
backup=0

POSITIONAL_ARGS=()
while [[ $# -gt 0 ]]; do
  case $1 in
    -y)
        dryrun=0
        shift
        ;;
    -b)
        backup=1
        shift
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

# read `find` result into array
rawlist=()
function find_all_raw() {
    OIFS=$IFS
    trap 'IFS=${OIFS}' RETURN
    IFS=$'\r\n'
    rawlist=($(eval "find * -not -path '*/.*' -not -path '*_RawBak/*' -type f \( ${raw_types} \) -print" | sort))
}
find_all_raw

cnt=0
for file in "${rawlist[@]}"; do
    if [ ! -e "${file%.*}.JPG" -a ! -e "${file%.*}.jpg" ]; then
        if ((dryrun)); then
            echo "$file"
        elif ((backup)); then
            backup_dir="$(dirname "$file")/_RawBak"
            mkdir -p  "$backup_dir"
            mv -v "$file" "${backup_dir}/"
        else
            rm -fv "$file"
        fi
        cnt=$((cnt+1))
    fi
done

printf "Processed %d ophan RAW files" $cnt
((dryrun)) && printf " (DRY RUN)\n" || printf "\n"

# case $1 in
#     y|-y)
#         find . -type f \( -iname '*.RAF' -o -iname '*.DNG' \) -exec bash -c 'file="{}"; test -e "${file%.*}.JPG" -o -e "${file%.*}.jpg" || rm -fv "{}"' \; | tee /dev/stderr | wc -l
#         ;;
#     *)
#         find . -type f \( -iname '*.RAF' -o -iname '*.DNG' \) -exec bash -c 'file="{}"; test -e "${file%.*}.JPG" -o -e "${file%.*}.jpg" || ls "{}"' \; | tee /dev/stderr | wc -l
#         ;;
# esac

