
/*
 * CS-252
 * shell.y: parser for shell
 *
 * This parser compiles the following grammar:
 *
 *	cmd [arg]* [> filename]
 *
 * you must extend it to understand the complete shell grammar
 *
 */

%code requires 
{
#include <string>

#if __cplusplus > 199711L
#define register      // Deprecated in C++11 so remove the keyword
#endif
}

%union
{
  char        *string_val;
  // Example of using a c++ type in yacc
  std::string *cpp_string;
}

%token <cpp_string> WORD
%token TWOGREAT NOTOKEN GREAT NEWLINE GREATGREAT GREATAMPERSAND GREATGREATAMPERSAND
AMPERSAND PIPE LESS

%{
//#define yylex yylex
#include <stdio.h>
#include "shell.hh"
#include <string.h>
#include <stdlib.h>
#include <cstdio>

void yyerror(const char * s);
int yylex();

%}

%%

goal:
  commands
  ;

commands:
  command
  | commands command
  ;

command: simple_command
       | pipe_list iomodifier_list NEWLINE{
	Shell::_currentCommand.execute();
	}
	;

simple_command:	
  command_and_args iomodifier_list background_optional NEWLINE {
   // printf("   Yacc: Execute command\n");
    Shell::_currentCommand.execute();
  }
  | NEWLINE 
  | error NEWLINE { yyerrok; }
  ;

command_and_args:
  command_word argument_list {
    Shell::_currentCommand.
    insertSimpleCommand( Command::_currentSimpleCommand );
  }
  ;

argument_list:
  argument_list argument
  | /* can be empty */
  ;

argument:
  WORD {
   // printf("   Yacc: insert argument \"%s\"\n", $1->c_str());
    Command::_currentSimpleCommand->insertArgument( $1 );\
  }
  ;

command_word:
  WORD {
   // printf("   Yacc: insert command \"%s\"\n", $1->c_str());
    Command::_currentSimpleCommand = new SimpleCommand();
    Command::_currentSimpleCommand->insertArgument( $1 );
  }
  ;

pipe_list:
	 pipe_list PIPE command_and_args
	| command_and_args
	;
iomodifier_list:
	       iomodifier_list iomodifier_opt
	|/* empty*/
	;
background_optional:
		   AMPERSAND{
	Shell::_currentCommand._background = true;
	}
;
iomodifier_opt:
  GREAT WORD {
  //  printf("   Yacc: insert output \"%s\"\n", $2->c_str());
    if(Shell::_currentCommand._outFile != 0){
	printf("Ambiguous output redirect.\n");
	}
    Shell::_currentCommand._outFile = $2;
   // Command::_currentSimpleCommand->count++;
  }

  | GREATGREAT WORD {
//	printf("   GreatGreat Word Yacc: insert output \"%s\"\n", $2->c_str());
	Shell::_currentCommand._outFile = $2;
	Shell::_currentCommand._append = true;
  }

  | GREATAMPERSAND WORD {
//	printf("   GreatAmpersand Word Yacc: insert output \"%s\"\n", $2->c_str());	
	Shell::_currentCommand._outFile = $2;
	Shell::_currentCommand._errFile = $2;
  }

  | LESS WORD {
	Shell::_currentCommand._inFile = $2;
  }
 
  | TWOGREAT WORD {
	Shell::_currentCommand._outFile = $2;
	Shell::_currentCommand._errFile = $2;
  } 
  | GREATGREATAMPERSAND WORD {
//	printf("   GreatGreatAmpersand Word Yacc: insert output \"%s\"\n", $2->c_str());
	Shell::_currentCommand._outFile = $2;
	Shell::_currentCommand._errFile = $2;
	Shell::_currentCommand._append = true;
  }
  ;

%%

void
yyerror(const char * s)
{
  fprintf(stderr,"%s", s);
}

#if 0
main()
{
  yyparse();
}
#endif
