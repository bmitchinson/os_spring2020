#!/bin/bash

# script takes ⟨stdoutfile⟩, ⟨stderr file⟩, and ⟨cwd⟩, followed by an arbitrary
# number of other arguments specifying a subcommand and its arguments. Run the
# subcommand with all specified arguments, redirecting the subcommands
# stdout -> stout_file
# stdeer -> dtdeer_file
# subcommands working directory (execute in subshell) to -> cwd

# script cannot change it's own cwd. (suggested to use subshell)
# nothing else should be printed into stdout or stdeer

# To refer to all the arguments specified by the user after the argument ⟨cwd⟩,
# I suggest using the bash array variable "${@:⟨n⟩}"

stdoutfile=$1;
if [ -z "$stdoutfile" ]
then
  echo "Arg 1: (\$stdoutfile) wasn't given."
  exit 1
fi

stderrfile=$2;
if [ -z "$stderrfile" ]
then
  echo "Arg 2: (\$stderrfile) wasn't given."
  exit 1
fi

cwd=$3;
if [ -z "$cwd" ]
then
  echo "Arg 3: (\$cwd) wasn't given."
  exit 1
fi

command=$4
args="${@:5}"

(cd $cwd && $command "${args}" > $stdoutfile 2>$stderrfile | tee ; ( exit ${PIPESTATUS[0]} ) ) 
echo "$?"

### Example: ./script.sh ../test2/out.txt ../test2/err.txt ../test2 echo "heyyo" "hi there"

# ****************
# Task6 Complete
# ****************