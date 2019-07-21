#include <cstdio>

#include "shell.hh"
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int yyparse(void);

void Shell::prompt() {
  if(isatty(0)){

  printf("myshell>");
  fflush(stdout);
  
  }
}

Command Command::_currentCommand;

int yyparse(void);
extern "C" void cntrlC(int sig){
        printf("\n");
//      Command::_currentSimpleCommand->Shell::prompt();
        Command::_currentCommand.prompt();
}

extern "C" void zombie(int sig){
        int pid;
        pid = wait3(0,0,NULL);

        while(waitpid(-1,NULL,WNOHANG) > 0);
}

extern "C" void controlC(int sig){
	printf("\n");
	while(waitpid(-1,NULL,WNOHANG) > 0);
}

void Command::prompt(){
        if(isatty(0)){
        printf("myshell>");
        fflush(stdout);
        }
}

int main() {
//  Shell::prompt();
  

  int check;

  struct sigaction sig1;

  sig1.sa_handler = zombie;

  sigemptyset(&sig1.sa_mask);
  sig1.sa_flags = SA_RESTART;

  check = sigaction(SIGCHLD, &sig1, NULL);

  if(check){
          perror("sigaction");
          exit(-1);
  }


  //Control C 
  struct sigaction sig2;

  sig2.sa_handler = controlC;

  sigemptyset(&sig2.sa_mask);
  sig2.sa_flags = SA_RESTART;

  check = sigaction(SIGINT, &sig2, NULL);

  if(check){
  	perror("sigaction");
	exit(-1);
  }
  Command::_currentCommand.prompt();
  yyparse();
}


Command Shell::_currentCommand;
