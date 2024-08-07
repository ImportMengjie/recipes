/*
* 给你一个升序的数组numbers，返回一个满足相加之和等于target的两个数的index。
* 示例：
  输入：numbers = [2,7,11,15], target = 9
  输出：[1,2]
  解释：2 与 7 之和等于目标数 9 。因此 index1 = 0, index2 = 1 。返回 [0, 1] 。
*/
#include <iostream>
#include <vector>

using namespace std;

vector<int> twoSum(const vector<int> &numbers, int target) {
    int left = 0, right = numbers.size() - 1;
    while (left < right) {
        int sum = numbers[left] + numbers[right];
        if (sum == target)
            return {left, right};
        else if (sum < target)
            left++;
        else
            right--;
    }
    return {-1, -1};
}

// ----- TEST -----

#define PRINT_CALL(func, args...)                                           \
    ({                                                                      \
        std::cout << "func: " << #func << ", args: " << #args << std::endl; \
        func(args);                                                         \
    })

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
    os << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

int main() {
    std::cout << "test1: " << PRINT_CALL(twoSum, {2, 7, 11, 15}, 22) << std::endl;
    std::cout << "test2: " << PRINT_CALL(twoSum, {2, 7, 11, 15}, 13) << std::endl;
    std::cout << "test3: " << PRINT_CALL(twoSum, {2, 7, 11, 15}, 1) << std::endl;
}
