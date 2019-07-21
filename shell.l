
/*
 *
 * CS-252 
 * shell.l: lexical analyzer for shell
 * You have to extend it.
 *
 */

%{

#include <unistd.h>
#include <cstring>
#include "y.tab.hh"
#include <sys/wait.h>
#include <string.h>
//#include "read-line.c"

////////////  Start added code ///////////

extern "C" char * read_line();


int mygetc(FILE * f) {
 static char *p;
 char ch;

 if (!isatty(0)) {
 // stdin is not a tty. Call real getc
   return getc(f);
 }

 // stdin is a tty. Call our read_line.
 if (p==NULL || *p == 0) {
   char * s = read_line();
   p = s;
 }

 ch = *p;
 p++;

 return ch;
}

#undef getc
#define getc(f) mygetc(f)

/////////// End added code ///////////

static  void yyunput (int c,char *buf_ptr  );

void myunputc(int c) {
  unput(c);
}

%}
%%

\n {
  return NEWLINE;
}

[ \t] {
  /* Discard spaces and tabs */
}

">" {
  return GREAT;
}

">>" {
   return GREATGREAT;
}

"2>" {
   return TWOGREAT;
}
">&" {
   return GREATAMPERSAND;
}

">>&" {
   return GREATGREATAMPERSAND;
}

"&" {
   return AMPERSAND;
}

"|" {
  return PIPE;
}

"<" {
   return LESS;
}
\"[^\n\"]*\" {
//	printf("test1\n");
	std::string str = "";

	int a;

	int b = 0;

	for(a = 0; yytext[a] != '\0'; a++){
//	printf("test2\n");
	if(yytext[a] == '"'){
//		printf("test3\n");
		continue;

	}
	str += yytext[a];
//	printf("%s\n",str);
	b++;
	}
//	printf("%s\n",str.c_str());
	yylval.cpp_string = new std::string(str);
	return WORD;
}

[^ \t\n&<>\|]*\\[^ \t\n]* {
	std::string str = "";

	int a;
	int b = 0;

	for(a = 0; a != strlen(yytext); a++){
	if(yytext[a] == 92){
	if(yytext[a+1] == 92){
	str += yytext[a+1];
	a=a+1;
	b=b+1;
	}
	}
	else{
	str += yytext[a];
	b=b+1;
	}
	}
	yylval.cpp_string = new std::string(str);
	return WORD;	
}

 `.*` {
/*	char buff[1024];

	while(read(0,buff+a,1) != 0){
	printf("%c\n", *(buff+a));
	a=a+1;
	}
	
	dup2(dout,1);
	dup2(din,0);

	close(din);
	close(dout);	
	}

	waitpid(0,NULL,0);
	
	a = 0;
	c = strlen(buff);
	
	while(c > a){
	if(buff[(c-1-a)] == '\n'){
	unput(' ');
	}
	
	else{
	unput(buff[(c-1-a)]);
	}
	a=a+1;
	}*/

/*	int length;
	length = strlen(yytext);
*/
	char *cmd;
	cmd = strdup((&yytext[1]));

	char *p = cmd;
	
/*	int sLength;
	sLength = strlen(cmd);
	cmd[sLength] = '\0';
*/
/*	if(cmd[yyleng-2] == '`'){
	cmd[yyleng-2] = '\0';
	}
*/
	while(*p != '`'){
	p++;
	}

	*p = '\0';

	strcat(cmd, "\nexit\n");
	int din;
	int dout;

	din = dup(0);
	dout = dup(1);

	int fdin[2];
	int fdout[2];

	pipe(fdin);
	pipe(fdout);

	write(fdin[1],cmd,strlen(cmd));
	close(fdin[1]);
/*	write(fdin[1],"\n",1);
	write(fdin[1],"exit",4);
	write(fdin[1],"\n",1);
*/	
	dup2(fdin[0],0);
	close(fdin[0]);

/*	close(fdin[1]);

	dup2(fdin[0],0);
	close(fdin[0]);

	dup2(fdin[1],1);
	close(fdout[1]);
*/
	dup2(fdin[1],1);
	close(fdin[1]);
	int ret;
	ret = fork();

	if(ret == 0){
	execvp("/proc/self/exe",NULL);
        _exit(1);
	}
	else if(0 > ret){
	perror("fork");
	exit(1);
	}
/*
	else if(!ret){
	execvp("/proc/self/exe",NULL);
	exit(1);
	}
*/
	else{
	while(waitpid(ret,NULL,0) == -1);
	dup2(din,0);
	dup2(dout,1);

	close(fdin[0]);
	close(fdout[1]);
	close(fdin[1]);

	char a[32000];
	char *buff;
	buff = a;
	/*int x;
	x = 0;

	while(read(fdout[0],&a,1))
	{
	if(a == '\n'){
	buff[x++] = ' ';
	}
	else{
	buff[x++] = a;
	}
	buff[x] = '\0';
	}

//	buff[x] = '\0';

	for(x = strlen(buff); x >= 0; x=x-1){
	unput(buff[x]);
	}*/
	while(read(fdout[0],buff,1)){
	if(*buff == '\n'){
	*buff = ' ';
	}
	buff++;
	} ;
	
	*buff = '\0';
	close(fdout[0]);
	char *p;
	p = buff + strlen(buff);

	while(p != buff){
	unput(*p);
	p--;
	}
	unput(*p);
}
}
((\\[\&\\\{\}\"(\>)(\<)])*([^| \t\n><&])*)+   {
  /* Assume that file names have only alpha chars */
  yylval.cpp_string = new std::string(yytext);
  return WORD;
}

"." {
	return NOTOKEN; //invalid character as an input
}
%%
