#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0
#define WR 1

void prime(int p[2]){

  close(p[WR]);

  int first=0;
  if(read(p[RD],&first,4)!=4){
    fprintf(2, "Error in child\n");
    exit(1);
  }

  printf("prime %d\n", first);

  int num;
  read(p[RD],&num,4);

  int new_p[2];
  pipe(new_p);

  if(fork() == 0){
    prime(new_p);
  }
  else{
    close(new_p[RD]);
    if(num % first) write(new_p[WR], &num, 4);

    while(read(p[RD], &num, 4)){
      if(num % first) write(new_p[WR], &num, 4);
    }

    close(p[RD]);
    close(new_p[WR]);
    wait((int *)0);
  }

  exit(0);

}



int main(int argc, const char *argv[]){

  int p[2];
  pipe(p);

  if(fork() == 0){
    //child

    prime(p);

  }
  else{
    close(p[RD]);
    for(int i=2;i<35;++i){
      if(write(p[WR],&i,4) != 4){
        fprintf(2, "Error in parent\n");
        exit(1);
      }
    }

    close(p[WR]);
    wait((int *)0);
    
    exit(0);
  }
  exit(0);

}