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

strace $command "${args}" >out.tmp 2>err.tmp

head -n -1 err.tmp | while read -r line
do
  echo "${line%%(*}" >> all.tmp
done

readarray -t calls < all.tmp

sort_uni=($(echo "${calls[@]}" | tr ' ' '\n' | sort -u | tr '\n' ' '))

cat out.tmp
printf '%s\n' "${sort_uni[@]}"

rm out.tmp err.tmp all.tmp

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