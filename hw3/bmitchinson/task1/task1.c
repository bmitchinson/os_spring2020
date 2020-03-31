#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int virtualSelf(char* req_addr);
void (*signal(int signum, void (*sighandler)(int)))(int);
void clean_exit_on_sig(int sig_num)
{
  exit(1);
}

int main(int arg_count, char** argv) {
  signal(11, clean_exit_on_sig);
  // Requested Addr
  char* req_addr_str = argv[1];
  int isInRange = virtualSelf(req_addr_str);

  // if (isInRange){
  //   // TODO Catch segfault on read
  // }
  sleep(5000);
  return 0;
}

int virtualSelf(char* req_addr_str) {
  // address string
  unsigned long req_addr = strtol(req_addr_str, NULL, 16);

  // read bash output
  FILE *maps_output;
  char m_scan[1035];

  pid_t pid = getpid();
  printf("----\n");
  #pragma GCC diagnostic ignored "-Wformat="
  printf("%lu\n", pid);
  printf("req_addr: %s\n", req_addr_str);
  printf("----\n");

  // store maps command to read the pid specific maps file
  char* m_cmd;
  size_t m_cmd_sz;
  #pragma GCC diagnostic ignored "-Wformat="
  m_cmd_sz = snprintf(NULL, 0, "cat /proc/%lu/maps | grep -e 'heap' -e 'task1'", pid);
  m_cmd = malloc(m_cmd_sz + 1);
  #pragma GCC diagnostic ignored "-Wformat="
  snprintf(m_cmd, m_cmd_sz+1, "cat /proc/%lu/maps | grep -e 'heap' -e 'task1'", pid);

  maps_output = popen(m_cmd, "r");

  /* Read the output a line at a time - output it. */
  while (fgets(m_scan, sizeof(m_scan), maps_output) != NULL) {
    char* beg = malloc(8 + 1); 
    memcpy(beg, &m_scan[0], 8);
    char* end = malloc(8 + 1); 
    memcpy(end, &m_scan[9], 8);

    printf("%s\n", beg);
    printf("%s\n", end);
    printf("%s", m_scan);

    if ((strtol(beg, NULL, 16) <= req_addr) && (req_addr < strtol(end, NULL, 16))){
      return 1;
    }
  }

  exit(1);
}
