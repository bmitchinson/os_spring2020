#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int arg_count, char** argv) {
  char* req_addr = argv[1];
  printf("%s\n", req_addr);
  return 0;
}