#!/bin/bash

# print to stdout the path of all files in (path)
# recursively with a name ending with (ext)
echo "****************"
echo "task1"
echo "****************"

path=$1; ext=$2;
# conditional needs spaces on edges
# General rule: quote it if it can either be empty or contain spaces 
#     (or any whitespace really) or special characters (wildcards). 
#     Not quoting strings with spaces often leads to the shell breaking apart 
#     a single argument into many.
if [ -z "$path" ]
then
  echo "Arg 1: (\$path) wasn't given."
  exit 1
fi
if [ -z "$ext" ]
then
  echo "Arg 2: (\$ext) wasn't given."
  exit 1
fi

echo "Finding files that end in \"$2\" in path: \"$1\""
echo "****************"
find $1 -name "*$2"
echo "****************"

