#include <iostream>
#include <vector>

int main() {
  uint32_t num = 1;
  char *ptr = reinterpret_cast<char *>(&num);
  uint32_t position_info_raw[16]{0};
  if (*ptr == 1) {
    std::cout << "Little endian" << std::endl;
  } else {
    std::cout << "Big endian" << std::endl;
  }
  return 0;
}
