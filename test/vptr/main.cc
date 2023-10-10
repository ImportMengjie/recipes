#include "mylib.h"

class MyRunMe : public RunMe {

public:
  void fun1() override { cout << "my fun1" << endl; }

  void fun2() override { cout << "my fun2" << endl; }
  void fun3() override { cout << "my fun3" << endl; }

  ~MyRunMe() { cout << "~MyRunMe()" << endl; }
};

int main() {
  MyRunMe r;
  call_run_me(&r);
}
