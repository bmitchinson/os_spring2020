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
void process_command(command);
char* path_then_args();
char** decode_hex_into_args(char* hex_string);

//global variables here

char** decode_hex_into_args(char* hex_string) {
  int letters = strlen(hex_string);
  char* ascii_string = malloc(letters);

  int arg_count = 1;
  int i = 0;
  int pos = 0;
  while ( i < letters ){
    if( hex_string[i] == '0' && hex_string[i+1] == '0' ){
      ascii_string[pos] = 32; // replace 00 with spaces for parse()
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
  ascii_args = malloc(arg_count * letters);

  pos = 0;
  for (int arg = 0; arg < arg_count; arg++){
    ascii_args[arg] = malloc(letters);
    int sub_pos = 0;
    while (pos < letters && ascii_string[pos] != ' ') {
      ascii_args[arg][sub_pos] = ascii_string[pos];
      sub_pos++;
      pos++;
    }
    pos++;
  }

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

void process_command(command cmd){
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) { // child process
      printf("New child process started %d\n", (int) getpid());
      // set alternate file streams
      // https://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/dup2.html
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wint-conversion"
      if (cmd.stdin[0]) {
        printf("REPLACING1");
        stdin = open(cmd.stdin, O_RDONLY, 0664);
      }
      if (cmd.stdout[0]) {
        printf("REPLACING2");
        stdout = open(cmd.stdout, O_WRONLY|O_CREAT|O_TRUNC, 0664);
      }
      if (cmd.stderr[0]) {
        printf("REPLACING3");
        stderr = open(cmd.stderr, O_WRONLY|O_CREAT|O_TRUNC, 0664);
      }
      #pragma GCC diagnostic pop
      // dup2(fileno(someopenfile), STDIN_FILENO);
      // dup2(fileno(someotherfile), STDOUT_FILENO);
      // dup2(fileno(somethirdopenfile), STDERR_FILENO);
      // fclose(someopenfile);
      // fclose(someotheropenfile);
      // fclose(somethirdopenfile);
      // execvp(args[0], args);

      if (cmd.niceness) {
        nice(cmd.niceness);
      }

      // if timeout, 
        // wrap command with timeout

      // if args
        // set args

      if (cmd.arguments[0]) {
        char** converted_args = decode_hex_into_args(cmd.arguments);
        printf("converted_args 0: %s\n", converted_args[0]);
      } else {
        printf("no args\n");
      }

      exit(3);

      // runin shit - execv(?)()
      // Reminder that "use_path" means you're given the file name only
      // When calling execv and any of its variants, the valued specified as 
      //     path and the value specified as argv[0] must always be the same.
      // --------------------------------
      // if use_path = 1, copy_env = 0
        // use execv(pe)() -> envp being an empty array to avoid propagation
          // execv(pe) will never work when copy_env is 0 since that rids 
          //     the path var. "You can ignore this issue?"
      // else if use_path = 1 and copy_env = 1
        // use execv(p)() 
      // else if use_path = 0 and copy_env = 1
       // use execv(_)()
      // else use_path = 0 and copy_env = 0
        // use execv(e) -> envp being an empty array to avoid propagation

      // if (cmd.use_path == 1 && cmd.copy_environment == 0){
      //   // execvpe();
      // }
      // else if (cmd.use_path == 1 && cmd.copy_environment == 1) {
      //   // execvp();
      // }
      // else if (cmd.use_path == 0 && cmd.copy_environment == 1) {
      //   // execv();
      // }
      // else { // (cmd.use_path == 0 && cmd.copy_enviornment == 0) {
      //   // execve(); // -> envp being an empty array to avoid propagation
      // }
  }
  else {
    // printf("parent is movin onward\n");
    // parent process
    // if wait
      // wait for PID (rc)
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

    /*
    process_command will:
    - get a parsed_command variable
    - create a child process
    - set file redirection, niceness, arguments, envirionment variables, ...
    
    - call a proper variant of execv


    - print when a child process is created and when any child process is terminated
    - if necessary, wait for the termination of the program
    */
    // print_parsed_command(parsed_command);
    process_command(parsed_command);

    
    free_command(parsed_command);
  }

  // Wait for all children:
  int status;
  pid_t term;
  while( (term = waitpid(-1, &status, 0)) > 0 ) {
    printf("Child process %d terminated with exit code %d\n", 
      term, WEXITSTATUS(status));
  }

  //remember to wait for the termination of all the child processes, regardless of the value of parsed_command.wait

    // details and hints 
    
    // if you're using execv, you need to inject
    //     the path value (env value?) as argument 0 to execv. the args from
    //     the command_file then come after as arg 1 and arg 2 ...

    // (-) niceness value fails unless shell is root

    // assume stdin file exists, assume dirs containing stdout and stderr exist
    //     and are writable. Since stdout + stderr exist, delete their contents on
    //     load with `open(path, O_WRONLY|O_CREAT|O_TRUNC, 0664)` and 
    //     `open(path, O_RDONLY, 0664)` for stdin

    // As soon as any child process is terminated, the shell has to print out 
    //     the pid of the terminated process together with its exit code

}

