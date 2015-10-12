#! /bin/bash

fullpath=`pwd`
prjName=`basename $fullpath`
targetFile="${HOME}/.indexer_files"


echo "[$prjName]" > $targetFile
echo "" >> $targetFile
echo "${fullpath}**/*.h" >> $targetFile
echo "${fullpath}**/*.hpp" >> $targetFile
echo "${fullpath}**/*.c" >> $targetFile
echo "${fullpath}**/*.cc" >> $targetFile
echo "${fullpath}**/*.cpp" >> $targetFile
echo "" >> $targetFile
echo "" >> $targetFile
echo "" >> $targetFile
