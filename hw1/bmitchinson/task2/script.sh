#!/bin/bash

echo "****************"
echo "task2"
echo "****************"
echo "Printing .txt files in path: \"$1\"..."
echo "****************"
for f in $(find $1 -name "*.txt")
do
  echo $f
  cat $f
done
echo "****************"