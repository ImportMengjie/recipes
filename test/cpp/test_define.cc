#include <iostream>

#define MOUNT_PATH "/data/"

#define MY_PATH MOUNT_PATH "abc/123"

#define DE1

using namespace std;

int main() {
  char my_char[] = "my" MY_PATH "456";

  cout << MY_PATH << endl;
  cout << my_char << endl;
  cout << MY_PATH "456" << endl;
#if defined DE2 || defined DE1
  cout << "define!" << endl;
#endif
}

