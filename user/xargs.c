#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define RD 0
#define WR 1


int read_one_line(char *new_argv[32], int current_argc){

  char buf[1024];
  int n=0;
  while(read(RD,buf+n,1) == 1){

    if(n == 511){
      fprintf(2, "command too long\n");
      exit(1);
    }

    if(buf[n] == '\n'){
      break;
    }
    n++;
  }

  buf[n] = 0;

  if(n == 0)  return 0;

  int offset = 0;
  //one row arg can be: 123 123\n, so we can split it 
  while(offset < n){
    new_argv[current_argc++] = buf + offset;

    while(buf[offset] != ' ' && offset<n){
      offset++;
    }
    while(buf[offset] != ' ' && offset<n){
      buf[offset++] = 0;
    }

  }

  return current_argc;

}









int main(int argc, char *argv[]){
  if(argc <= 1){
    fprintf(2, "usage: xargs command (arg ..)\n");
    exit(0);
  }

  char *command = malloc(strlen(argv[1])+1);
  char *new_argv[MAXARG];

  strcpy(command, argv[1]);
  for(int i=1;i<argc;++i){
    new_argv[i-1] = malloc(strlen(argv[i]+1));
    strcmp(new_argv[i-1], argv[i]);
  }

  int current_argc;
  // read_one_line uses to append args to current new_argv 
  while((current_argc = read_one_line(new_argv, argc-1))!=0){
    new_argv[current_argc] = 0;
    if(fork()==0){
      exec(command, new_argv);
      fprintf(2, "exec failed\n");
      exit(1);
    }
    else{
      wait((int *)0);
    }
  }
  exit(0);


}