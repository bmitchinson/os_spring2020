#include <stdio.h>
#include <sys/user.h>
#include <err.h>
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

  // byte* mem;
  char* assembly_code_raw = argv[1];
  byte *mem = mmap(0, 10000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (mem == MAP_FAILED){
    printf("map failed\n");
    exit(1);
  }
  else {
    printf("we're good\n");
    // sscanf("h", "%c", mem[0]);
    // sscanf("i", "%c", mem[1]);
    // sscanf("\0", "%c", mem[2]);
    int placed = sscanf("hi", "%s", mem);
    printf("placed: %i\n", placed);
    // sscanf(assembly_code_raw[0], "%02hx", mem[0]);
    printf("scanned\n");
    printf("mem: %c\n", mem[1]);
    printf("k\n");
    // ((void (*)(void))mem)();
  }
  // sscanf("e", "%02hx", mem);
  // for (int i = 0; i < strlen(assembly_code_raw); i++){
  // }
}
