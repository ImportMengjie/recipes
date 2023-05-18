#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

#define N 100000
using namespace std;
mutex mu;
condition_variable cv;
int num = 0;

void printNum(int threadIdx, function<bool(const int &)> printCond) {
    unique_lock<mutex> lck(mu);  // 放在这里也是ok的, cv.wait(lck)会释放锁,
                                 // 所以不会发生一直未释放锁;
    while (true) {
        while (!printCond(num)) cv.wait(lck);
        if (num < N) std::cout << "Thread" << threadIdx << ": " << num << endl;
        num++;
        cv.notify_all();
        if (num >= N) break;
    }
}

int main() {
    cout << "------" << endl;
    num = 0;
    std::thread t2(printNum, 2, [](const int &x) { return x % 2 != 0; });
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    std::thread t1(printNum, 1, [](const int &x) { return x % 2 == 0; });
    t1.join();
    t2.join();
    cout << "------" << endl;

    return 0;
}