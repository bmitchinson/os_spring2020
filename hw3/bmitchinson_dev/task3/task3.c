#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int arg_count, char** argv) {
  int fd = open(argv[1], O_RDWR);
  struct stat sb;
  fstat(fd,&sb);
  long file_size = sb.st_size;

  char *file_in_mem = mmap(0, file_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  msync(file_in_mem, file_size, MS_SYNC);
  close(fd);

  char rev[file_size + 2];
  file_size = file_size - 1;
  for (int i=file_size; i >= 0; i--){
    rev[file_size - i] = (char) file_in_mem[i];
    printf("c:%c\n", file_in_mem[i]);
  }
  rev[file_size + 1] = '\0';
  printf("rev:%s\n", rev);

  fd = fopen(argv[1], "w+");
  fputs(rev, fd);
  close(fd);
}