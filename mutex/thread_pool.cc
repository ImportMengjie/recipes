#include <chrono>
#include <condition_variable>
#include <deque>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class ThreadPool {
public:
    explicit ThreadPool(int thread_num) {
        threads.reserve(thread_num);
        for (size_t i = 0; i < thread_num; i++) threads.emplace_back(&ThreadPool::threadFunc, this);
    }

    void postTask(const std::function<void()> &task) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (stop) return;
            tasks.push_back(task);
        }
        cv.notify_all();
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stop = true;
        }
        cv.notify_all();
        for (std::thread &t : threads) t.join();
    }

private:
    void threadFunc() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                cv.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                if (this->stop) return;
                task = std::move(tasks.front());
                tasks.pop_front();
            }
            task();
        }
    }

    std::mutex mutex_;
    std::deque<std::function<void()>> tasks;
    std::condition_variable cv;
    std::vector<std::thread> threads;
    bool stop{false};
};

int main() {
    ThreadPool pool(4);
    for (int i = 0; i < 10; i++) {
        pool.postTask([i] {
            std::cout << "task: " << i << ", id: " << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "finish task: " << i << std::endl;
        });
    }
    {
        int i = 0;
        std::cin >> i;
    }
}
