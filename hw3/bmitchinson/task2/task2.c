#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <sys/mman.h>

typedef unsigned char byte;

int main(int arg_count, char** argv) {
  // char* assembly_code_raw = argv[1];
  // void* mem = malloc((strlen(assembly_code_raw) / 2) + 1);
  // printf("%p\n",mem);
  //sscanf("hey", "%s", mem);
  // sscanf(assembly_code_raw, "%02hhx", mem);
  // ((void (*)(void))mem)();
  // printf("%s\n",mem);
  // sscanf(assembly_code_raw, "%02hhx", mem);

  byte* mem;
  char* assembly_code_raw = argv[1];
  mem = (byte*) mmap(0, 100, PROT_EXEC|PROT_READ|PROT_WRITE, MAP_SHARED, -1, 0);
  sscanf(assembly_code_raw, "%02hx", mem);
  ((void (*)(void))mem)();
}
