#include <stdio.h>

int main(int arg_count, char** argv) {
  for (int i = 0; i < arg_count; i++){
    printf("%s\n", argv[i]);
  }
  return 0;
}