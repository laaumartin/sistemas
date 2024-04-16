#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
int main(void){
 int fd[2];
 pid_t pid;
 if (pipe(fd) < 0) {
   perror("pipe");
   exit(-1);
 }
 pid = fork();
 switch(pid) {
   case -1: /* error */
     perror("fork");
     exit(-1);
  case 0: /*child process executes ls*/
     close(fd[0]);
     close(STDOUT_FILENO);
     dup(fd[1]);
     close(fd[1]);
     execlp("ls","ls",NULL);
     perror("execlp");
     exit(-1);
  default: /*father process executes wc*/
     close(fd[1]);
     close(STDIN_FILENO);
     dup(fd[0]);
     close(fd[0]);
     execlp("wc","wc",NULL);
     perror("execlp");
 }
}
