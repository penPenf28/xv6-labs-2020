#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


#define RD 0
#define WR 1

int main(int argc, const char* argv[]){
  int p1[2];
  int p2[2];

  if(pipe(p1) < 0){
      printf("pipe(p1) failed\n");
      exit(1);
  }

  if(pipe(p2) < 0){
      printf("pipe(p2) failed\n");
      exit(1);
  }


  if(fork() == 0){
    //child, close write port of p1 and read port of p2
    //after deliver a char, close read port of p1 and write port of p2

    close(p1[WR]);
    close(p2[RD]);
    char c[2];

    if(read(p1[RD], c, 1) != 1){
      fprintf(2, "Error read\n");
      exit(1);
    }

    int pid = getpid();
    printf("%d: received ping\n", pid);

    if(write(p2[WR], c, 1) != 1){
      fprintf(2, "Error write\n");
      exit(1);
    }

    close(p1[RD]);
    close(p2[WR]);

  }
  else{
    //parent, opposite to child
    close(p1[RD]);
    close(p2[WR]);
    char c[2] = "a\0";

    if(write(p1[WR], c, 1) != 1){
      fprintf(2, "Error write\n");
      exit(1);
    }

    wait((int *)0);

    int pid = getpid();
    printf("%d: received pong\n", pid);

    if(read(p2[RD], c, 1) != 1){
      fprintf(2, "Error read\n");
      exit(1);
    }

    close(p1[WR]);
    close(p2[RD]);

  }


  exit(0);


}