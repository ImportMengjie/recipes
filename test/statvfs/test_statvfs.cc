#include <cstdio>
#include <iostream>
#include <sys/statvfs.h>

using namespace std;

int main(int argc, char **argv) {
  struct statvfs fs;
  if (statvfs(argv[1], &fs) == 0) {
    cout << "f_bsize: " << fs.f_bsize << ", f_frsize: " << fs.f_frsize
         << ", f_blocks: " << fs.f_blocks << ", f_bfree: " << fs.f_bfree
         << ", f_bavail: " << fs.f_bavail << endl;
    cout << "f_files: " << fs.f_files << ", f_ffree: " << fs.f_ffree
         << ", f_favail: " << fs.f_favail << endl;
    cout << "f_fsid: " << fs.f_fsid << ", f_flag" << fs.f_flag << ", f_namemax"
         << fs.f_namemax << endl;
  } else {
    perror("statvfs");
  }
}
