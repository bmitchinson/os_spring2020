FROM ubuntu:16.04

RUN apt-get update && apt-get upgrade -y

RUN apt install build-essential -y

RUN apt-get install valgrind -y

WORKDIR /app

# docker build . -t os
# docker run -it -v $(pwd):/app os

# gcc -Wall -pedantic -g shell.c -o shell
# valgrind ./shell ls.txt