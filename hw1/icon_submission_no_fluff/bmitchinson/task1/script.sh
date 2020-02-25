#!/bin/bash

# print to stdout the path of all files in ($path)
# recursively with a name ending with ($ext)
path=$1; ext=$2;
# conditional needs spaces on edges
# General rule: quote it if it can either be empty or contain spaces 
#     (or any whitespace really) or special characters (wildcards). 
#     Not quoting strings with spaces often leads to the shell breaking apart 
#     a single argument into many.

find $1 -name "*$2"

### Example: ./script.sh ../test .txt

# ****************
# task1
# ****************
# Finding files that end in ".txt" in path: "test"
# ****************
# test/sub1/textfile1.txt
# test/sub1/sub3/textfile3.txt
# test/textfile2.txt
# ****************