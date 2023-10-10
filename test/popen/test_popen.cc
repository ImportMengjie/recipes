#include <limits.h>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  string files;
  //char buf[PIPE_BUF];
  char buf[10];
  for (int i = 1; i < argc; i++) {
    files += string(argv[i]) + " ";
  }
  string cmd = files;
  std::cout << cmd << std::endl;
  FILE *fp = popen(cmd.c_str(), "r");
  if (fp == NULL) {
    perror("popen");
    exit(1);
  }
  while (fgets(buf, sizeof(buf), fp) != NULL) {
    std::cout << buf;
    std::flush(std::cout);
  }
  pclose(fp);
}
