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
  byte* mem = (byte*) mmap(0, 10000, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  byte* mem_start = mem;
  printf("mem: %p\n", mem);
  printf("mem_start: %p\n", mem_start);
  
  if (mem == MAP_FAILED){
    printf("map failed\n");
    exit(1);
  }
  else {
    int i = 0;
    char* two_hex = malloc(2);
    while (i < strlen(assembly_code_raw)){
      two_hex[0] = assembly_code_raw[i];
      two_hex[1] = assembly_code_raw[i+1];
      printf("two_hex:%s\n", two_hex);
      sscanf(two_hex, "%02hhx", mem);
      printf("wrote: %s\n", mem);
      mem++;

      i+=2;
    }
    printf("mem:       %p\n", mem);
    printf("mem_start: %p\n", mem_start);

    ((void (*)(void))mem_start)();

    // sscanf("h", "%c", mem);
    // mem++;
    // sscanf("i", "%c", mem);
    // mem++;
    // sscanf("\0", "%c", mem);
    // printf("placed: %i\n", placed);
    // printf("scanned\n");
    // mem--;
    // mem--;
    // printf("mem: %c\n", mem);
    // printf("k\n");
  }
  // sscanf("e", "%02hx", mem);
  // }
}
