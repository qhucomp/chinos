// init: The initial user-level program

#include "kernel/include/types.h"
#include "kernel/include/stat.h"
#include "kernel/include/file.h"
#include "kernel/include/fcntl.h"
#include "xv6-user/user.h"
char *argv[] = {"lua",0};
int
main(void)
{
  dev(O_RDWR, CONSOLE, 0);
  dup(0);  // stdout
  dup(0);  // stderr
  // printf("testing...\n");
  // int pid = clone();
  // if(pid == 0)
    execve("lua",argv);
  // else
    // printf("parent\n");
  while(1);
}
