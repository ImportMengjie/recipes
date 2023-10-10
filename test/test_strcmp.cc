#include <string.h>
#include <stdio.h>

struct MyStruct {
  size_t size;
  char buffer[2];
};

void print(char* buffer) {
  printf("%d\n", buffer);
}

int main() {
  int a = -2;
  printf("uint=>int: %ud,%ud \n", static_cast<unsigned int>(a), (unsigned int)a);
  printf("%s\n", NULL);
  MyStruct *my = NULL;
  print(my->buffer);
  // printf("%d\n", strcmp("aaa",NULL));
}

