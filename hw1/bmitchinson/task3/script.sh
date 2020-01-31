#!/bin/bash

# Given ($dir), the script must print
# {# of files contained in dir}{space}{# of subdirs in $dir}
echo "****************"
echo "task3"
echo "****************"

dir=$1;
if [ -z "$dir" ]
then
  echo "Arg 1: (\$path) wasn't given."
  exit 1
fi

echo "$(find $dir -type f | wc -l | tr -d [:space:]) $(find $dir -type d | wc -l | tr -d [:space:])"
echo "****************"