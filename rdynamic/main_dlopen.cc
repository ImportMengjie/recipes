#include <dlfcn.h>
#include <stdio.h>

typedef void (*func_t)(void);

void foo() { printf("main dlopen foo\n"); }

int main() {
  setbuf(stdout, 0);
  printf("call bar from so\n");
  void *handler = dlopen("./libbar.so", RTLD_LAZY);
  printf("dlopen %d\n", handler);

  func_t bar_func = dlsym(handler, "_Z3barv");
  bar_func();

  dlclose(handler);
}
