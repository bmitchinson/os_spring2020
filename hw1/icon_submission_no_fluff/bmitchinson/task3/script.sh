#!/bin/bash

# Given ($dir), the script must print
# {# of files contained in dir}{space}{# of subdirs in $dir including itself}

dir=$1;

find $dir -type d

echo "$(find $dir -type f | wc -l | tr -d [:space:]) $(find $dir -type d | wc -l | tr -d [:space:])"

### Example: ./script ../test

# ****************
# task3
# ****************
# 6 4
# ****************