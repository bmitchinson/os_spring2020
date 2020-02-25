#!/bin/bash

# Script to print name + content of any txt file in ($dir) recursively

dir=$1;

# this for loop runs on each line returned from find. 
for f in $(find $1 -name "*.txt")
do
  cat $f
done

### Example: `./script.sh test`

# ****************
# task2
# ****************
# Printing .txt files in path: "test"...
# ****************
# test/sub1/textfile1.txt
# test/sub1/sub3/textfile3.txt
# contents of textfile 3
# textfile 1 is blank
# test/textfile2.txt
# contents of textfile 2
# hello :)
# textfile 1 is blank
# ****************