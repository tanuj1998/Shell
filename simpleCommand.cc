#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include <pwd.h>

#include "simpleCommand.hh"

SimpleCommand::SimpleCommand() {
  _arguments = std::vector<std::string *>();
  count = 0;
 /* num_of_avail_args = 0;
  num_of_args = 5;
  _arg =(char**)malloc(num_of_avail_args * sizeof(char *));
*/
  }

SimpleCommand::~SimpleCommand() {
  // iterate over all the arguments and delete them
  for (auto & arg : _arguments) {
    delete arg;
  }
}
/*
char * SimpleCommand::tilde(char * argument){
	if(argument[0] == '~'){
	if(strlen(argument) == 1){
	argument = strdup(getenv("HOME"));
	return argument;
	}

	else{
	if(argument[1] == '/'){
		char *directory;
		directory = strdup(getenv("HOME"));

		argument=argument+1;
		argument = strcat(directory,argument);

		return argument;
	}

	char * new_arg;
	char *name;
	char *tmp;
	char *usr;

	new_arg = (char*)malloc(strlen(argument)+20);
	name = (char*)malloc(50);

	usr = name;
	tmp = argument;

	tmp=tmp+1;

	while(*tmp != '/' && *tmp){
	*(name++) = *(tmp++);
	}
	*name = '\0';

	if(*tmp){
	new_arg = strcat(getpwnam(usr)->pw_dir,tmp);
	argument = strdup(new_arg);
	return argument;
	}
	else{
	argument = strdup(getpwnam(usr)->pw_dir);
	return argument;
	}
	}
	}
	return NULL;
}
char * SimpleCommand::checkExp(char * argument){
	char *args;
	char *dollar_check;
	char *brace_check;
	args = strdup(argument);
	dollar_check = strchr(args, '$');
	brace_check = strchr(args, '{');

	char * rp = (char *)malloc(sizeof(argument) + 50);

	char *tmp;
	tmp = rp;

	if(dollar_check && brace_check){
	while(*args != '$')
	{
	*tmp = *args;
	tmp=tmp+1;
	args=args+1;
	}
	*tmp = '\0';

	while(dollar_check)
	{
	if(dollar_check[1] == '{' && dollar_check[2] != '}'){
		char *temp;
		char *env;
		char *temp_env;

		temp = dollar_check+2;
		env = (char*) malloc(30);
		temp_env = env;

		while(*temp != '}'){
		*temp_env = *temp;
		temp_env=temp_env+1;
		temp = temp+1;
		}

		*temp_env = '\0';

		char *gt;
		gt = getenv(env);

		strcat(rp,gt);

		while( *(args-1) != '}'){
		args = args+1;
		}

		char *buff;
		char *temp_buff;

		buff = (char*)malloc(30);
		temp_buff = buff;

		while(*args != '$' && *args){
		*temp_buff = *args;
		temp_buff=temp_buff+1;
		args=args+1;
		}

		*temp_buff = '\0';

		strcat(rp,buff);
		}

		dollar_check = dollar_check+1;
		dollar_check = strchr(dollar_check, '$');
	}
	argument = strdup(rp);
	return argument;
	}
	return NULL;
}*/
void SimpleCommand::insertArgument( std::string * argument ) {
  // simply add the argument to the vector
 _arguments.push_back(argument);
/*
	if(num_of_avail_args == num_of_args + 1){
	_arg  = (char **) realloc(_arg, num_of_avail_args * sizeof(char*));
	}
  char * expression;
  expression = checkExp(argument);

  if(expression)
	  argument = strdup(expression);

  _arg[ num_of_args ] = argument;

  _arg[ num_of_args + 1] = NULL;

  num_of_args = num_of_args+1;
*/
}


// Print out the simple command
void SimpleCommand::print() {
  for (auto & arg : _arguments) {
    std::cout << "\"" << *arg << "\" \t";
  }
  // effectively the same as printf("\n\n");
  std::cout << std::endl;
}
