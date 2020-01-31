#!/bin/bash

# Script to print name + content of any txt file in ($dir) recursively
echo "****************"
echo "task2"
echo "****************"

dir=$1;
if [ -z "$dir" ]
then
  echo "Arg 1: (\$dir) wasn't given."
  exit 1
fi

echo "Printing .txt files in path: \"$1\"..."
echo "****************"
# this for loop runs on each line returned from find. 
for f in $(find $1 -name "*.txt")
do
  echo $f
  cat $f
done
echo "****************"