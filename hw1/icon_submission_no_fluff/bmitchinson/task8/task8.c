#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* hex_encode(char *asci);
char int_to_hex_char(int input);

int main(int arg_count, char** argv) {
  for (int i = 0; i < arg_count; i++){
    char* hex_res=( hex_encode(argv[i]) );
    printf("%s\n", hex_res);
  }
  return 0;
}

char int_to_hex_char(int input){
  if (input > 9) {
      return input + 87;
    }
    else {
      return input + 48;
    }
}

char* hex_encode(char *word){
  int letters = strlen(word);
  int coded_size = (letters * 2) + 1;
  char *coded = malloc(coded_size);

  int j = 0;
  for (int i = 0; i < letters; i++){
    int ascii_val = (int) word[i];

    int mod1 = ascii_val % 16;
    int next = ascii_val / 16;
    int mod2 = next % 16;
    coded[j] = int_to_hex_char(mod2);
    coded[j+1] = int_to_hex_char(mod1);
    j+=2;
  }
  coded[coded_size-1] = '\0';

  return coded;
}


/*
Example: 13 to hex (0D)
13 % 16 -> 13 -> D second pos
^ aka 13 mod 16? does 0 mod 16 yield 0? If so that's it no edge case.
0 % 16 -> 0 -> first pos

Example: 101 to hex (65)
101 / 16 -> 6 r 5
6 / 16 -> 0 r 6
*/