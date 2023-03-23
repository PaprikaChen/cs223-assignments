/*----------------------------------------------
 * Author: Parika Chen
 * Date: 2023/3/22
 * This program does a practice with fork()
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
  pid_t pid;
  printf("%d] A0 \n", getpid());
  pid = fork();
  if (pid == 0) { // child 1
    printf("%d] B1 \n", getpid());
  }
  else { // parent
    printf("%d] B0 \n", getpid());
    pid = fork();
    if (pid == 0) { // child 2
      printf("%d] C1 \n", getpid());
    }
    else printf("%d] C0 \n", getpid());
  }
  printf("%d] Bye \n", getpid());

  return 0;
}
