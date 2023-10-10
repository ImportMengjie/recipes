#include <stdio.h>

extern void foo();

void bar() {
  printf("call so bar()\n");
  foo();
}

