#include <stdio.h>

void bar();

typedef void (*func_t)(void);

void foo() {
  printf("main foo\n");
}

int main() {
  setbuf(stdout, 0);
  printf("main call bar\n");
  bar();
}

