#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

class Data1 {
private:
    std::mutex mutex_;
    std::vector<int> data_;

public:
    std::vector<int> getData() {
        std::lock_guard<std::mutex> guard(mutex_);
        return data_;
    }

    void addDataItem(int v) {
        std::lock_guard<std::mutex> guard(mutex_);
        data_.push_back(v);
    }

    void setData(std::vector<int>&& data) {
        // 右值直接move
        std::lock_guard<std::mutex> guard(mutex_);
        data_ = std::move(data);
    }

    void setData(const std::vector<int>& data) {
        // 左值先copy后move
        auto tmp = data;  // copy放在临界区外
        setData(std::move(tmp));
    }
};

class Data {
private:
    std::mutex mutex_;
    std::shared_ptr<std::vector<int>> data_;

public:
    Data() : data_(new std::vector<int>()) {}

    std::shared_ptr<const std::vector<int>> getDataRef() {
        std::lock_guard<std::mutex> guard(mutex_);
        // 返回shared_ptr的拷贝，引用计数器+1
        return data_;
    }

    void addDataItem(int v) {
        std::lock_guard<std::mutex> guard(mutex_);
        // 在有读时才copy
        if (!data_.unique()) data_.reset(new std::vector<int>(*data_));
        data_->push_back(v);
        assert(data_.unique());
    }

    void setData(std::vector<int>&& data) {
        // 右值直接move
        std::lock_guard<std::mutex> guard(mutex_);
        if (!data_.unique())
            data_.reset(new std::vector<int>(std::move(data)));
        else
            *data_ = std::move(data);
        assert(data_.unique());
    }

    void setData(const std::vector<int>& data) {
        // 左值先copy后move
        auto tmp = data;  // copy放在临界区外
        setData(std::move(tmp));
    }
};

int main() {
    Data d;
    for (int i = 0; i < 10; ++i) d.addDataItem(i);

    auto vec = d.getDataRef();
    for (auto v : *vec) {
        std::cout << v << ", ";
    }
    std::cout << std::endl;
    d.setData({4, 5, 6});
    for (auto v : *vec) {
        std::cout << v << ", ";
    }
    std::cout << std::endl;
}
