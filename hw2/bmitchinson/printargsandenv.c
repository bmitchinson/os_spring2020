#include <stdio.h>

// `char *argv[]` === `char** argv`
int main(int arg_count, char** argv, char** env) {
  for (int i = 0; i < arg_count; i++){
    printf("%s\n", argv[i]);
  }
  int i = 0;
  while(env[i] != NULL){
    printf("%s\n", env[i]);
    i++;
  }
  return 0;
}