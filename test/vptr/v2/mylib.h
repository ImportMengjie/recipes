#include <iostream>

using namespace std;

class RunMe {
public:
  virtual void fun1() { cout << "base func1" << endl; }

  virtual void fun2() { cout << "base func2" << endl; }

  virtual void fun2_5() { cout << "base func2.5" << endl; }

  virtual void fun3() { cout << "base func3" << endl; }

  virtual ~RunMe() { cout << "base ~RunMe()" << endl; }
};

void call_run_me(RunMe *r);
