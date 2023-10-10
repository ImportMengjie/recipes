#include <future>
#include <memory>
#include <thread>
#include "memory"
#include <chrono>

using namespace std;

class my{
  public:
    std::future<void> f;
};


void start(std::shared_ptr<my> ptr) {
  std::this_thread::sleep_for(std::chrono::seconds(10));
  ptr.reset();
}


int main() {
  {
    auto ptr = std::make_shared<my>();
    ptr->f = std::async(std::launch::async,start, ptr);
  }
  std::this_thread::sleep_for(std::chrono::seconds(1000));
}
