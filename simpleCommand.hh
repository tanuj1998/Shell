#ifndef simplcommand_hh
#define simplecommand_hh

#include <string>
#include <vector>

struct SimpleCommand {

  // Simple command is simply a vector of strings
  std::vector<std::string *> _arguments;

  SimpleCommand();
  ~SimpleCommand();
  void insertArgument( std::string * argument );
  char * checkExp(char * argument);
  char * tilde(char * argument);
  void print();
  int count;

  int num_of_args;
  int num_of_avail_args;
  char ** _arg;
};

#endif
