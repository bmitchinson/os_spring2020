#!/bin/bash

# Create a tar that compresses everything in ($dir), and saves it in the format
# "($tag)_(unixTimeInSeconds).tar"
echo "****************"
echo "task4"
echo "****************"

dir=$1;
if [ -z "$dir" ]
then
  echo "Arg 1: (\$dir) wasn't given."
  exit 1
fi

tag=$2;
if [ -z "$tag" ]
then
  echo "Arg 2: (\$tag) wasn't given."
  exit 1
fi

time=$(date +%s)

echo "Creating $2_$time.tar"

tar -cf "$2_$time.tar" -C $1 .

echo "$2_$time.tar created"
echo "****************"

### Example: ./script.sh ../test fake_name

# ****************
# task4
# ****************
# Creating fake_name_1580759103.tar
# fake_name_1580759103.tar created
# ****************