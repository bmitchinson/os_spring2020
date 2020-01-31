#!/bin/bash

echo "****************"
echo "task1"
echo "****************"
echo "Finding files that end in \"$2\" in path: \"$1\"..."
echo "****************"
find $1 -name "*$2"
echo "****************"

