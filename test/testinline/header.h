#ifndef HEADER
#define HEADER
#include <stdio.h>

static void hello() {
  static int i = 0;
  printf("%d\n", i++);
}
#endif // !HEADER

