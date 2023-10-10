#include <iostream>
class A {
public:
  int a;
  A() : a(0x1) {}
  virtual void foo() { std::cout << "A::foo()" << std::endl; }
  virtual void boo() { std::cout << "A::foo()" << std::endl; }
  void bar() { std::cout << "A::bar()" << std::endl; }
};
class B : public A {
public:
  int b;
  B() : A(), b(0x2) {}
  void foo() { std::cout << "B::foo()" << std::endl; }
};
int main() {
  A a;
  B b;
  A *p = &b;
  p->foo();
  std::cout << sizeof(int) << " " << sizeof(int *) << std::endl;
  return 0;
}
