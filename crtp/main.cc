#include <iostream>

using namespace std;

template <typename T> class Base {
public:
  void SomeFunc() {
    T &derived = static_cast<T &>(*this);
    // T* derived = static_cast<T*>(this);
    ////或者用指针的方式
    derived.DoSomething();
  }
};

class Derived : public Base<Derived> {
public:
  void DoSomething() { cout << "derived do DoSomething" << endl; }
};

int main() {
  Base<Derived> *b = new Derived();
  b->SomeFunc();
}

