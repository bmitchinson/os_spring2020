#!/bin/bash

# Copy all executable files from ($source) (recursively) to ($dest).
# Assume $dest exists
echo "****************"
echo "task5"
echo "****************"

source=$1;
if [ -z "$dir" ]
then
  echo "Arg 1: (\$source) wasn't given."
  exit 1
fi

dest=$2;
if [ -z "$tag" ]
then
  echo "Arg 2: (\$dest) wasn't given."
  exit 1
fi

