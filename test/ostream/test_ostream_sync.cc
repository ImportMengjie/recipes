#include <fstream>
#include <iostream>
#include <string>
using namespace std;
#include <unistd.h>

int GetFileDescriptor(std::filebuf &filebuf) {
  class my_filebuf : public std::filebuf {
  public:
    int handle() { return _M_file.fd(); }
  };

  return static_cast<my_filebuf &>(filebuf).handle();
}

int main(void) {
  fstream f("tmp.txt", ios::out);
  f.write("test", 4);
  cout << "wait write" << endl;
  // f.flush(); //清空缓冲区
  cout << "sync: " << f.sync() << endl;
  fsync(GetFileDescriptor(*f.rdbuf()));
  cout << "wait sync" << endl;
  f.close();
  return 0;
}
