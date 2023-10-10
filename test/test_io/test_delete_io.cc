#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  char buf[1024]{0};
  char filename[] = "./tmp.txt";
  FILE *fd = fopen(filename, "wb+");
  setbuf(fd, NULL);
  remove(filename);
  for (int i = 0; i < 1000; i++) {
    sprintf(buf, "%d ", i);
    fwrite(buf, strlen(buf), 1, fd);
  }
  printf("fsync: %d\n",fsync(fd->_fileno));
  // fclose(fd);
  // fd = fopen(filename, "rb");
  sync();
  fseek(fd, 0, SEEK_SET);
  size_t read_size = 0;
  while ((read_size = fread(buf, 1, sizeof(buf)-1, fd)) != 0) {
    buf[read_size] = '\0';
    printf("read %ld: %s;", read_size, buf);
  }
  fclose(fd);
}
