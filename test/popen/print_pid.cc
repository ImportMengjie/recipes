#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
  printf("start main \n");
  for(int i=0;i<10;i++) {
   // while(true) {
    printf("%d my exec pid is: %d\n", i,getpid());
    fflush(stdout);
    sleep(1);
    //usleep(1000000);
  }
  return 0;
}
