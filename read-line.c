/*
 * CS252: Systems Programming
 * Purdue University
 * Example that shows how to read one line with simple editing
 * using raw terminal.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER_LINE 2048

extern void tty_raw_mode(void);

// Buffer where line is stored
int line_length;
char line_buffer[MAX_BUFFER_LINE];

// Simple history array
// This history does not change. 
// Yours have to be updated.
int history_index = 0;
int point;
char *history[1000];
int history_length = sizeof(history)/sizeof(char *);

void read_line_print_usage()
{
  char * usage = "\n"
    " ctrl-?       Print usage\n"
    " Backspace    Deletes last character\n"
    " up arrow     See last command in the history\n";

  write(1, usage, strlen(usage));
}

/* 
 * Input a line with some basic editing.
 */
char * read_line() {

  // Set terminal in raw mode
  tty_raw_mode();

  line_length = 0;

  // Read one line until enter is typed
  while (1) {

    // Read one character in raw mode.
    char ch;
    read(0, &ch, 1);

    if (ch>=32 && ch <= 126) {
      // It is a printable character. 

      // Do echo
      write(1,&ch,1);

      // If max number of character reached return.
      if (line_length==MAX_BUFFER_LINE-2) break; 

      // add char to buffer.
      line_buffer[line_length]=ch;
      line_length++;
      point=point+1;
    }
    else if (ch==10) {
      // <Enter> was typed. Return line
      
      // Print newline
      write(1,&ch,1);

      break;
    }
    else if (ch == 31) {
      // ctrl-?
      read_line_print_usage();
      line_buffer[0]=0;
      break;
    }
    else if (ch == 8 || ch == 127) {
      // <backspace> was typed. Remove previous character read.

	if(point == 0 || line_length == 0){ continue;}
      
	int a;
	a = point;

	while(line_length > a){
	line_buffer[a-1] = line_buffer[a];
	a = a+1;
	}

	line_length = line_length-1;
	point = point - 1;

	ch = ' ';
	write(1,&ch,1);
      // Go back one character
      
      ch = 8;
      write(1,&ch,1);

      ch = 8;
      write(1,&ch,1);

      int b;
      b = point;

      while(line_length > b){
      ch = line_buffer[b];
      write(1,&ch,1);
      b=b+1;
      }
      // Write a space to erase the last character read
      ch = ' ';
      write(1,&ch,1);

      // Go back one character
      ch = 8;
      write(1,&ch,1);

      int c;
      c = point;

      while(line_length > c){
      ch = 8;
      write(1,&ch,1);
      c = c+1;
      }
      // Remove one character from buffer
//      line_length--;
    }

    else if(ch == 4){
    if(point == line_length){ continue;}
	
    ch = ' ';
    write(1,&ch,1);

    ch = 8;
    write(1,&ch,1);

    int d;
    d = point;

    while(line_length > d){
    line_buffer[d] = line_buffer[d+1];
    d=d+1;
    }
    line_length = line_length-1;

    int e;
    e = point;

    while(line_length > e){
    ch = line_buffer[e];
    write(1,&ch,1);
    e=e+1;
    }

    ch = ' ';
    write(1,&ch,1);

    ch = 8;
    write(1,&ch,1);

    int f;
    f = point;

    while(line_length > f){
    ch = 8;
    write(1,&ch,1);
    f=f+1;
    }
    }

    else if(ch == 1){
    while(0 < point){
    	char x;
        char y;
	char z;
 	x = 27;

	y = 91;
	z = 68;
	write(1, &x, 1);

	write(1, &y, 1);
	
	write(1, &z, 1);
	
	point = point-1;
    }
    }

    else if(ch == 5){
    while(line_length > point){
    	char x;
        char y;
	char z;
	
	x = 27;
        y = 91;
	z = 67;

        write(1, &x, 1);

        write(1, &y, 1); 
       
        write(1, &z, 1); 
        
	point=point+1;
    }
    }
    else if (ch==27) {
      // Escape sequence. Read two chars more
      //
      // HINT: Use the program "keyboard-example" to
      // see the ascii code for the different chars typed.
      //
      char ch1; 
      char ch2;
      read(0, &ch1, 1);
      read(0, &ch2, 1);
      if (ch1==91 && ch2==65) {
	// Up arrow. Print next line in history.

	// Erase old line
	// Print backspaces
	int i = 0;
	for (i =0; i < line_length; i++) {
	  ch = 8;
	  write(1,&ch,1);
	}

	// Print spaces on top
	for (i =0; i < line_length; i++) {
	  ch = ' ';
	  write(1,&ch,1);
	}

	// Print backspaces
	for (i =0; i < line_length; i++) {
	  ch = 8;
	  write(1,&ch,1);
	}	

	// Copy line from history
	if(0 < history_length && history_index >= 0){
	
	strcpy(line_buffer, history[history_index--]);
        	
        history_index=(history_index)%history_length;
	
	if(history_index == -1){
	history_index = history_length -1;
	}

	line_length = strlen(line_buffer);
	}


//	strcpy(line_buffer, history[history_index]);
//	line_length = strlen(line_buffer);
//	history_index=(history_index+1)%history_length;

	// echo line
	write(1, line_buffer, line_length);
      }

	else if((ch1 == 91) && (ch2 == 66)){
	int x;
	
	for(x = 0; line_length > x; x=x+1){
	ch = 8;
	write(1,&ch,1);
	}	
	//on top
	for(x = 0; line_length > x; x=x+1){
	ch = ' ';
	write(1,&ch,1);
	}

	//backspaces
	for(x = 0; line_length > x; x=x+1){
	ch = 8;
	write(1,&ch,1);
	}
	if(history_length == history_index){
	history_index = history_length-1;

	strcpy(line_buffer,"");
	}
	else if(0 < history_length && (history_length-1) >= history_index){
	strcpy(line_buffer,history[history_index++]);
	}

	line_length = strlen(line_buffer);

	write(1,line_buffer,line_length);
	}      

      else if(ch1 == 91 && ch2 == 68){
      if(point == 0){ continue; }

      else{
      ch = 8;
      write(1,&ch,1);
      point = point-1;
      }
      }

      else if(ch1 == 91 && ch2 == 67){
      if(line_length == point){ continue; }

      else{
      ch = line_buffer[point];
      write(1,&ch,1);
      point=point+1;
      }
      }

      else if(ch == 5){
      if(line_length == point || line_length == 0){ continue;}

      int a;
      a = 0;

      for(a = point; line_length > a; a=a+1){
        char x;
	char y;
	char z;

	 x = 27;
	write(1, &x, 1);
	
	 y = 91;
	write(1, &y, 1);
	
	 z= 67;
	write(1, &z, 1);
      }
      point = line_length;
      }
    }

  }

  // Add eol and null char at the end of string
  line_buffer[line_length]=10;
  line_length++;
  line_buffer[line_length]=0;

  history[history_length] = (char*)malloc(strlen(line_buffer)*sizeof(char)+1);

  strcpy(history[history_length++], line_buffer);

  history[history_length - 1][strlen(line_buffer)-1] = '\0';

  history_index = history_length - 1;

  free(history[history_length]);

  return line_buffer;
}

