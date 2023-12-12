// Jayanth Anala - ja4874
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define MAX_SIZE 1024
char glbuff[MAX_SIZE];

void tail2(int fd, int ls){
  int n,i,numLines = 0,remLines = 0;
  char *tempbuff = (char*)malloc(500000);
  int bufsize = 0;
  while ((n = read(fd, glbuff, sizeof(glbuff))) > 0) {
    for (i=0; i<n; i++){
      tempbuff[bufsize] = (char)glbuff[i];
      bufsize++;
      if(glbuff[i] == '\n')
        numLines++;
    }
  }

  if(n<0){
    printf(1, "-- cannot open --\n");
    exit();
  }

  if(numLines<ls)
    remLines = 0;
  remLines = numLines - ls;
  int lc=0;
  for(i=0; i<bufsize; i++) {
    if(lc>=remLines){
      printf(1,"%c",tempbuff[i]);
    }
    if(tempbuff[i]=='\n'){
      lc++;
    }
  }
  free(tempbuff); //no memory laek
}

int main (int argc, char *argv[]) {
  int lines = 10,fd = -1,i,j;
  if(argc<=1){
    tail2(0, lines);
    exit();
  }else if(argc>1 && argv[1][0]=='-'){
    char linesNum[10];
    for (j=1; j<strlen(argv[1]); j++){
      linesNum[j-1] = argv[1][j];
    }
    lines = (atoi)(linesNum);
  }else{
    if((fd = open(argv[1], lines)) < 0){
      printf(1, "-- cannot open --%s\n", argv[1]);
      exit();
    }
    tail2(fd, lines);
    close(fd);
  }

  if(argc>2){
    for (i=2; i<argc; i++){
      if((fd = open(argv[i], 0))<0){
        printf(1, "-- cannot open --%s\n", argv[i]);
        exit();
      }else{
        tail2(fd, lines);
        close(fd);
      }
    }
  }
  exit();
}
