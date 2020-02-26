FROM ubuntu:16.04

RUN apt-get update && apt-get upgrade

RUN apt install build-essential

RUN apt-get install valgrind

WORKDIR /app

# docker run -it -v $(pwd):/app os_container

# gcc -Wall -pedantic -g shell.c -o shell
# valgrind ./shell ls.txt