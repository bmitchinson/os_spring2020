#!/bin/bash

# Copy all executable files from ($source) (recursively) to ($dest).
# Assume $dest exists
echo "****************"
echo "task5"
echo "****************"

source=$1;
if [ -z "$source" ]
then
  echo "Arg 1: (\$source) wasn't given."
  exit 1
fi

dest=$2;
if [ -z "$dest" ]
then
  echo "Arg 2: (\$dest) wasn't given."
  exit 1
fi

echo "Copying executables to folder: \"$dest\""
for exec_file in $(find $source -type f -perm -u+x); do
  echo "Copied $exec_file"
  cp $exec_file $2
done
echo "****************"

### Example: ./script.sh execs output

# ****************
# task5
# ****************
# Copying executables to folder: "output"
# Copied execs/sub1/sub2/e_three.sh
# Copied execs/e_one.sh
# ****************