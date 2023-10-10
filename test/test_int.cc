#include <chrono>
#include <cstdint>
#include <iostream>
#include <limits>

using namespace std;

template <class T>
bool add(T &a, const std::chrono::system_clock::time_point &start,
         const string &s) {
  static bool print = false;
  if (a < numeric_limits<T>().max()) {
    a++;
    return true;
  }
  if (!print) {
    print = true;
    std::cout << typeid(a).name() << " end, use time: "
              << std::chrono::duration_cast<chrono::minutes>(
                     std::chrono::system_clock::now() - start)
                     .count()
              << ", type: " << s << std::endl;
  }
  return false;
}

int main() {
  int i32 = 0;
  uint u32 = 0;
  uint64_t u64 = 0;
  int64_t i64 = 0;
  double d64 = 0;
  float f32 = 0;
  auto start = std::chrono::system_clock::now();
  bool stop = false;
  while (!stop) {
    bool stop = true;
    if (add(i32, start, "i32")) {
      stop = false;
    }
    if (add(u32, start, "u32")) {
      stop = false;
    }
    if (add(u64, start, "u64")) {
      stop = false;
    }
    if (add(i64, start, "i64")) {
      stop = false;
    }
    if (add(d64, start, "d64")) {
      stop = false;
    }
    if (add(f32, start, "f32")) {
      stop = false;
    }
  }
}
