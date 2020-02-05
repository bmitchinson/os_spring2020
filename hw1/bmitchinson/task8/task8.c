#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* hex(char *asci);

int main(int arg_count, char** argv) {
  for (int i = 0; i < arg_count; i++){
    char* hex_res=( hex(argv[i]) );
    printf("%s\n", hex_res);
  }
  return 0;
}

char* hex(char *word){
  int letters = strlen(word);
  int coded_size = (letters * 2) + 1;
  char *coded = malloc(coded_size);

  for (int i = 0; i < coded_size; i+=2){
    // The only edge case in the 2 value assumption is less than 10 (16 dec) (includes \n)
    coded[i] = '-';
    coded[i+1] = '_';
  }
  coded[coded_size-1] = '\0';

  return coded;
}

/*
Example 13 to hex (0D)
13 / 16 -> 13 -> D second pos
^ aka 13 mod 16? does 0 mod 16 yield 0? If so that's it no edge case.
0 / 16 -> 0 -> first pos
*/