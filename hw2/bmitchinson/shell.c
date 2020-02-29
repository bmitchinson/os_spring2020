#define _GNU_SOURCE //this is needed to be able to use execvpe 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <fcntl.h>

#include <signal.h>


typedef struct {
  char* binary_path;
  char* stdin;
  char* stdout;
  char* stderr;
  char* arguments;
  char* extra_environment;
  short use_path;
  short copy_environment;
  short niceness;
  short wait;
  unsigned int timeout;
} command;

//function prototypes
void print_parsed_command(command);
short parse_command(command*, char*);
void process_command(command, char**);
char* path_then_args();
char** decode_hex_into_args(char* hex_string, char* binary_path);
char** decode_hex_into_env(char* hex_string);

char** decode_hex_into_args(char* hex_string, char* binary_path) {
  int letters = strlen(hex_string);
  char* ascii_string = malloc(letters);

  int arg_count = 1;
  if (binary_path) {
    arg_count++;
  }
  int i = 0;
  int pos = 0;
  while ( i < letters ){
    if( hex_string[i] == '0' && hex_string[i+1] == '0' ){
      ascii_string[pos] = 64;
      arg_count++;
    }
    else {
      int asc = 0;
      asc = (hex_string[i] - 48) * 16;
      if (hex_string[i+1] > 57) {
        asc += hex_string[i+1] - 87;
      } else {
        asc += hex_string[i+1] - 48;
      }
      ascii_string[pos] = asc;
    }
    pos++;
    i+=2;
  }

  char** ascii_args;
  ascii_args = malloc((arg_count * letters) + 1);

  pos = 0;
  for (int arg = 0; arg < arg_count; arg++){
    if (binary_path && arg == 0) {
      ascii_args[0] = malloc(strlen(binary_path));
      strcpy(ascii_args[0], binary_path);
    } else {
      ascii_args[arg] = malloc(letters);
      int sub_pos = 0;
      while (pos < (letters / 2) && ascii_string[pos] != '@') {
        ascii_args[arg][sub_pos] = ascii_string[pos];
        sub_pos++;
        pos++;
      }
      ascii_args[arg][sub_pos] = '\0';
      pos++;
    }
  }
  ascii_args[arg_count] = NULL;
  return ascii_args;
}

char** decode_hex_into_env(char* hex_string) {
  int letters = strlen(hex_string);
  char* ascii_string = malloc(letters);

  int arg_count = 1;
  int i = 0;
  int pos = 0;
  while ( i < letters ){
    if( hex_string[i] == '0' && hex_string[i+1] == '0' ){
      ascii_string[pos] = 32;
      arg_count++;
    }
    else {
      int asc = 0;
      asc = (hex_string[i] - 48) * 16;
      if (hex_string[i+1] > 57) {
        asc += hex_string[i+1] - 87;
      } else {
        asc += hex_string[i+1] - 48;
      }
      ascii_string[pos] = asc;
    }
    pos++;
    i+=2;
  }

  char** ascii_args;
  ascii_args = malloc((arg_count * letters) + 1);

  pos = 0;
  for (int arg = 0; arg < arg_count; arg++){
    ascii_args[arg] = malloc(letters);
    int sub_pos = 0;
    while (pos < (letters / 2) && ascii_string[pos] != ' ') {
      ascii_args[arg][sub_pos] = ascii_string[pos];
      sub_pos++;
      pos++;
    }
    ascii_args[arg][sub_pos] = '\0';
    pos++;
  }
  ascii_args[arg_count] = NULL;
  return ascii_args;
}

short getlinedup(FILE* fp, char** value){
  char* line = NULL;
  size_t n = 0;
  //get one line
  int ret = getline(&line, &n, fp);

  if(ret == -1){
    //the file ended
    return 0;
  }
  //remove \n at the end
  line[strcspn(line, "\n")] = '\0';
  //duplicate the string and set value
  *value = strdup(line);
  free(line);

  return 1;
}

//parse a command_file and set a corresponding command data structure
short parse_command(command* parsed_command, char* cmdfile){
  FILE* fp = fopen(cmdfile, "rb");
  if(!fp){
    //the file does not exist
    return 0;
  }

  char* value;
  short ret;
  int intvalue;

  ret = getlinedup(fp,&value);
  if(!ret){
    fclose(fp); return 0;
  }
  parsed_command->binary_path = value;

  ret = getlinedup(fp,&value);
  if(!ret){
    fclose(fp); return 0;
  }
  parsed_command->stdin = value;

  ret = getlinedup(fp,&value);
  if(!ret){
    fclose(fp); return 0;
  }
  parsed_command->stdout = value;

  ret = getlinedup(fp,&value);
  if(!ret){
    fclose(fp); return 0;
  }
  parsed_command->stderr = value;

  ret = getlinedup(fp,&value);
  if(!ret){
    fclose(fp); return 0;
  }
  parsed_command->arguments = value;

  ret = getlinedup(fp,&value);
  if(!ret){
    fclose(fp); return 0;
  }
  parsed_command->extra_environment = value;

  ret = getlinedup(fp,&value);
  if(!ret){
    fclose(fp); return 0;
  }
  intvalue = atoi(value);
  if(intvalue != 0 && intvalue != 1){
    fclose(fp); return 0;
  }
  parsed_command->use_path = (short)intvalue;

  ret = getlinedup(fp,&value);
  if(!ret){
    fclose(fp); return 0;
  }
  intvalue = atoi(value);
  if(intvalue != 0 && intvalue != 1){
    fclose(fp); return 0;
  }
  parsed_command->copy_environment = (short)intvalue;

  ret = getlinedup(fp,&value);
  if(!ret){
    fclose(fp); return 0;
  }
  intvalue = atoi(value);
  if(intvalue <-20 || intvalue >19){
    fclose(fp); return 0;
  }
  parsed_command->niceness = (short)intvalue;

  ret = getlinedup(fp,&value);
  if(!ret){
    fclose(fp); return 0;
  }
  intvalue = atoi(value);
  if(intvalue != 0 && intvalue != 1){
    fclose(fp); return 0;
  }
  parsed_command->wait = (short)intvalue;

  ret = getlinedup(fp,&value);
  if(!ret){
    fclose(fp); return 0;
  }
  intvalue = atoi(value);
  if(intvalue < 0){
    fclose(fp); return 0;
  }
  parsed_command->timeout = (unsigned int)intvalue;

  return 1;
}

//useful for debugging
void print_parsed_command(command parsed_command){
  printf("----------\n");
  printf("binary_path: %s\n", parsed_command.binary_path);
  printf("stdin: %s\n", parsed_command.stdin);
  printf("stdout: %s\n", parsed_command.stdout);
  printf("stderr: %s\n", parsed_command.stderr);
  printf("arguments: %s\n", parsed_command.arguments);
  printf("extra_environment: %s\n", parsed_command.extra_environment);
  printf("use_path: %d\n", parsed_command.use_path);
  printf("copy_environment: %d\n", parsed_command.copy_environment);
  printf("niceness: %d\n", parsed_command.niceness);
  printf("wait: %d\n", parsed_command.wait);
  printf("timeout: %d\n", parsed_command.timeout);
}

void free_command(command cmd){
  free(cmd.binary_path);
  free(cmd.stdin);
  free(cmd.stdout);
  free(cmd.stderr);
  free(cmd.arguments);
  free(cmd.extra_environment);
}

void process_command(command cmd, char* existing_env[]){
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) { // child process
      printf("New child process started %d\n", (int) getpid());
      
      if (cmd.stdin[0]) {
        int fd = open(cmd.stdin, O_RDONLY, 0664);
        dup2(fd, 0);
        close(fd);
      }
      if (cmd.stdout[0]) {
        int fd = open(cmd.stdout, O_WRONLY|O_CREAT|O_TRUNC, 0664);
        dup2(fd, 1);
        close(fd);
      }
      if (cmd.stderr[0]) {
        int fd = open(cmd.stderr, O_WRONLY|O_CREAT|O_TRUNC, 0664);
        dup2(fd, 2);
        close(fd);
      }

      if (cmd.niceness) {
        nice(cmd.niceness);
      }      

      char* command;
      command = malloc(strlen(cmd.binary_path));
      strcpy(command, cmd.binary_path);
      char** converted_args = NULL;
      if (cmd.arguments[0] && cmd.use_path == 0) {
        converted_args = decode_hex_into_args(cmd.arguments, command);
      } else if (cmd.arguments[0]) {
        converted_args = decode_hex_into_args(cmd.arguments, command);
        // converted_args = decode_hex_into_args(cmd.arguments, NULL);
      } else {
        printf("no args\n");
      }

      if (cmd.timeout != 0) {
        char** decoded_args;
        command = malloc(20);
        strcpy(command, "/usr/bin/timeout");
        decoded_args = decode_hex_into_args(cmd.arguments, cmd.binary_path);

        converted_args = malloc(200);
        converted_args[0] = "/usr/bin/timeout";
        converted_args[1] = "--preserve-status";
        converted_args[2] = "-k";
        converted_args[3] = "1";
        converted_args[4] = malloc(5);
        sprintf(converted_args[4], "%d", cmd.timeout);
        for (int i = 0; decoded_args[i] != NULL; i++){
          converted_args[5+i] = malloc(strlen(decoded_args[i]) + 1);
          strcpy(converted_args[5+i], decoded_args[i]);
        }
      }

      if (cmd.use_path == 1 && cmd.copy_environment == 0) {
        char** custom_env = NULL;
        if (cmd.extra_environment[0]) {
          custom_env = decode_hex_into_env(cmd.extra_environment);
        }
        // TODO to merge with existing?
        execvpe(command, converted_args, custom_env);
      } else if (cmd.use_path == 1 && cmd.copy_environment == 1) {
        // printf("exist0:%s\n", existing_env[0]);
        // printf("cmd:%s\na0:%s\na1:%s\na2:%s\na3:%s\na4:%s\n",command, converted_args[0], converted_args[1], converted_args[2], converted_args[3], converted_args[4]);
        execvpe(command, converted_args, existing_env);
      } else if (cmd.use_path == 0 && cmd.copy_environment == 1) {

      }
      else if (cmd.use_path == 0 && cmd.copy_environment == 0) {
        execve(command, converted_args, NULL);
      }
      printf("*** exit3 should never happen ***\n");
      exit(3);
  }
  else {
    // parent process
    if (cmd.wait > 0) {
      int status;
      pid_t term;
      while( (term = waitpid(rc, &status, 0)) > 0 ) {
        printf("Child process %d terminated with exit code %d\n", 
          term, WEXITSTATUS(status));
      }
    }
  }
}

int main(int argc, char *argv[], char* env[]) {

  for(int ncommand=1; ncommand<argc; ncommand++){
    command parsed_command;
    int ret = parse_command(&parsed_command, argv[ncommand]);
    if (!ret){
      printf("command file %s is invalid\n", argv[ncommand]);
      continue;
    }
    
    process_command(parsed_command, env);
    free_command(parsed_command);
  }

  // Wait for all children:
  int status;
  pid_t term;
  while( (term = waitpid(-1, &status, 0)) > 0 ) {
    printf("Child process %d terminated with exit code %d\n", 
      term, WEXITSTATUS(status));
  }

    // if you're using execv, you need to inject
    //     the path value (env value?) as argument 0 to execv. the args from
    //     the command_file then come after as arg 1 and arg 2 ...

    // (-) niceness value fails unless shell is root

    // assume stdin file exists, assume dirs containing stdout and stderr exist
    //     and are writable. Since stdout + stderr exist, delete their contents on
    //     load with `open(path, O_WRONLY|O_CREAT|O_TRUNC, 0664)` and 
    //     `open(path, O_RDONLY, 0664)` for stdin

}

