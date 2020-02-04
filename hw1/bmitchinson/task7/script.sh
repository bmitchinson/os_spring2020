#!/bin/bash

# script takes ⟨[command, arguments]⟩ 
# Run the subcommand with all specified arguments while monitoring it with
# strace. redirect the st

echo "****************"
echo "Task7 Complete"
echo "****************"

command=$1;
if [ -z "$command" ]
then
  echo "Arg 1: (\$command) wasn't given."
  exit 1
fi

args="${@:2}"

# echo "strace "$command $args""

strace $command "${args}" 2>temp.txt

### Example: ./script.sh uname -m

# ****************
# Task7
# ****************
# x86_64
# access
# arch_prctl
# brk
# close
# execve 
# exit_group
# fstat
# mmap
# mprotect
# munmap
# open
# read
# uname
# write