/*
 * CS252: Shell project
 *
 * Template file.
 * You will need to add more code here to execute the command table.
 *
 * NOTE: You are responsible for fixing any bugs this code may have!
 *
 * DO NOT PUT THIS PROJECT IN A PUBLIC REPOSITORY LIKE GIT. IF YOU WANT 
 * TO MAKE IT PUBLICALLY AVAILABLE YOU NEED TO REMOVE ANY SKELETON CODE 
 * AND REWRITE YOUR PROJECT SO IT IMPLEMENTS FUNCTIONALITY DIFFERENT THAN
 * WHAT IS SPECIFIED IN THE HANDOUT. WE OFTEN REUSE PART OF THE PROJECTS FROM  
 * SEMESTER TO SEMESTER AND PUTTING YOUR CODE IN A PUBLIC REPOSITORY
 * MAY FACILITATE ACADEMIC DISHONESTY.
 */

#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "command.hh"
#include "shell.hh"
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

extern FILE *yyin;
extern int yyparse();

Command::Command() {
    // Initialize a new vector of Simple Commands
    _simpleCommands = std::vector<SimpleCommand *>();

    _outFile = NULL;
    _inFile = NULL;
    _errFile = NULL;
    _background = false;
    _append = false;
}

void Command::insertSimpleCommand( SimpleCommand * simpleCommand ) {
    // add the simple command to the vector
    _simpleCommands.push_back(simpleCommand);
}

void Command::clear() {
    // deallocate all the simple commands in the command vector
    for (auto simpleCommand : _simpleCommands) {
        delete simpleCommand;
    }

    // remove all references to the simple commands we've deallocated
    // (basically just sets the size to 0)
    _simpleCommands.clear();

    if ( _outFile ) {
        delete _outFile;
    }
    _outFile = NULL;

    if ( _inFile ) {
        delete _inFile;
    }
    _inFile = NULL;

    if ( _errFile ) {
        delete _errFile;
    }
    _errFile = NULL;

    _background = false;
}

int Command::builtIn(int a){
  if(!strcmp(_simpleCommands[a]->_arguments[0]->c_str(), "unsetenv")){
  int check;
  //set check to unsetenv and print error+clear
  check = unsetenv(_simpleCommands[a]->_arguments[1]->c_str());

  if(check)  {
  perror("unsetenv");
  }
  clear();
  Shell::prompt();
  return 1;
}

if(!strcmp(_simpleCommands[a]->_arguments[0]->c_str(), "setenv")){
	int check;
	check = setenv(_simpleCommands[a]->_arguments[1]->c_str(), _simpleCommands[a]->_arguments[2]->c_str(), 1);

	if(check){
	perror("setenv");
	}

	clear();
	Shell::prompt();
	return 1;

}
/*
if(!strcmp(_simpleCommands[a]->_arguments[0]->c_str(), "printenv")){
	char **en;
	en = environ;

	while(*en != NULL){
	printf("%s\n",*en);
	en = en+1;
	}

	clear();
	Shell::prompt();
//	return 1;
	exit(1);
}*/

  if(!strcmp(_simpleCommands[a]->_arguments[0]->c_str(), "cd")){
  int check;

  if(_simpleCommands[a]->_arguments.size() != 1){
  check = chdir(_simpleCommands[a]->_arguments[1]->c_str());
  }
  else{
  check = chdir(getenv("HOME"));
  }

  if(0 > check){
  perror("cd: can't cd to notfound\n");
  }

  clear();
  Shell::prompt();
  return 1;
  }
return 0;
}
void Command::print() {
    printf("\n\n");
    printf("              COMMAND TABLE                \n");
    printf("\n");
    printf("  #   Simple Commands\n");
    printf("  --- ----------------------------------------------------------\n");

    int i = 0;
    // iterate over the simple commands and print them nicely
    for ( auto & simpleCommand : _simpleCommands ) {
        printf("  %-3d ", i++ );
        simpleCommand->print();
    }

    printf( "\n\n" );
    printf( "  Output       Input        Error        Background\n" );
    printf( "  ------------ ------------ ------------ ------------\n" );
    printf( "  %-12s %-12s %-12s %-12s\n",
            _outFile?_outFile->c_str():"default",
            _inFile?_inFile->c_str():"default",
            _errFile?_errFile->c_str():"default",
            _background?"YES":"NO");
    printf( "\n\n" );
}

void Command::execute() {
    // Don't do anything if there are no simple commands
    if ( _simpleCommands.size() == 0 ) {
        Shell::prompt();
        return;
    } 


    // check if exit command
    if(!strcmp(_simpleCommands[0]->_arguments[0]->c_str(), "exit")){
	printf("Good bye\n");
    	    exit(1);
//	printf("Shell Exits");	    
	}
    // Print contents of Command data structure
  //  print();

    // Add execution here
    // For every simple command fork a new process
    // Setup i/o redirection
    // and call exec
	
    	int min;
	int mout;
	
	min = dup(0);
	mout = dup(1);

	int fin, fout, ferr;
        int ret;
       	int terr;

	terr = dup(2);

	if(!_inFile){
	fin = dup(min);
	}

	else{
	fin = open(_inFile->c_str(), O_RDONLY, 0664);
	}

	if(!_errFile){
	ferr = dup(terr);
	}

        else if(_errFile){
    	
	if(!_append){
	ferr = open(_errFile->c_str(), O_CREAT|O_WRONLY|O_TRUNC, 0664);
	}

	else if(_append){
	ferr = open(_errFile->c_str(), O_CREAT|O_WRONLY|O_APPEND, 0664);
	} 
    }

    dup2(ferr,2);
    close(ferr);

    unsigned int a;
    for(a = 0; _simpleCommands.size() > a; a++){
    dup2(fin,0);
    close(fin);

    if(builtIn(a)){
    return;
    }
    if((_simpleCommands.size()-1) == a){
	    if(!_outFile){
	    fout = dup(mout);
	    }

	    else if(_outFile){

	    if(!_append){
	    fout = open(_outFile->c_str(), O_CREAT|O_WRONLY|O_TRUNC, 0664);
	    }

	    else if(_append){
	    fout = open(_outFile->c_str(), O_CREAT|O_WRONLY|O_APPEND, 0664);
	    }
	    }
    }

    else if((_simpleCommands.size()-1) != a){
    int fd_pipe[2];
    pipe(fd_pipe);

    fout = fd_pipe[1];
    fin = fd_pipe[0];
    }

    dup2(fout,1);
    close(fout);

    ret = fork();

    if( 0 > ret){
    perror("fork");
    return;
    }

    else if(ret == 0){
 
   if(!strcmp(_simpleCommands[a]->_arguments[0]->c_str(), "source")){
	int n;
	n = _simpleCommands[a]->_arguments.size();
	if(2 > n){
	perror("not enough arguments!");
	exit(1);
	}

	else{
		FILE *f1;
        f1 = fopen(_simpleCommands[a]->_arguments[1]->c_str(), "r+");
        FILE *f2;
        f2 = yyin;

        yyin = f1;

	clear();
//  	perror("Test");
	yyparse();
        yyin = f2;
        fclose(f1); // close the file
	}


	}


    else if(!strcmp(_simpleCommands[a]->_arguments[0]->c_str(), "printenv")){
    char **en;

    en = environ;

    while(*en){
        printf("%s\n", *en);
        en = en+1;
    }
    clear();
    Shell::prompt();
    exit(1);
    }   

   else{
    char *arr[400];

    unsigned int b;
    
    for(b = 0; _simpleCommands[a]->_arguments.size() > b; b++){
    arr[b] =(char*) _simpleCommands[a]->_arguments[b]->c_str();

    }

    execvp(_simpleCommands[a]->_arguments[0]->c_str(),arr);
    perror("execvp");
    _exit(1);
   }
    }
    }
    dup2(min,0);
    dup2(mout,1);
   // dup2(terr,2);

    close(min);
    close(mout);
   // close(terr);

    if(!_background){
    waitpid(ret, NULL, 0);
    }
    // Clear to prepare for next command
    clear();

    // Print new prompt
    Shell::prompt();
}

SimpleCommand * Command::_currentSimpleCommand;

