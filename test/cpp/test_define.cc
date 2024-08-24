#include <iostream>
#include <map>
#include <string>
#include <vector>

#define PI 3.14
#define PATH "/home/"
#define DOCUMENTS_PATH PATH "Documents/"
#define LONG_STR \
    "There is a long string you must deal with it \
as two line of sentence \n"

#define MAX(x, y) x > y ? x : y

#define MAX1(x, y) ((x) > (y) ? (x) : (y))

#define MAX2(x, y)          \
    ({                      \
        auto&& _x = x;      \
        auto&& _y = y;      \
        (void)(&_x == &_y); \
        _x > _y ? _x : _y;  \
    })

#define AREA(x, y) std::cout << "x: " << #x << ", y: " #y << ", area: " << x * y << std::endl;

#define B1 1
#define B2 2

#define A(x) a##x

#define B(x) B##x

#define LOG_LEVEL 0

#define DEBUG_LOG \
    if (LOG_LEVEL >= 1) printf("debug!\n");

#define DEBUG_LOG1                              \
    {                                           \
        if (LOG_LEVEL >= 1) printf("debug!\n"); \
    }

#define DEBUG_LOG2                              \
    do {                                        \
        if (LOG_LEVEL >= 1) printf("debug!\n"); \
    } while (0)

#define LOG(fmt, ...) printf(fmt, __VA_ARGS__)  // C99标准
#define LOG2(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define LOG3(fmt, myargs...) printf(fmt, ##myargs)  // GNU C扩展

struct buffer {
    int len;
    char a[0];
};

struct flexible_array{
    int len;
    char a[];
};

int main() {
    buffer* buf = (buffer*)malloc(sizeof(buffer) + 10);
    buf->len = 10;
    strcpy(buf->a, "hello");
    printf("a: %s\n", buf->a);
    LOG("hello %s\n", "world");
    // LOG("hello\n"); // 不能编译
    LOG2("hello %s\n", "world");
    LOG2("hello\n");
    LOG3("hello %s\n", "world");
    int i = 0, j = 2;
    std::cout << "MAX(1!=1,1!=2) = " << (MAX(1 != 1, 1 != 2)) << std::endl;
    std::cout << "3+MAX(1,2) = " << (3 + MAX(1, 2)) << std::endl;

    std::cout << "MAX(i++,++j) = " << MAX1(i++, ++j) << std::endl;
    std::cout << "MAX(i,j) = " << MAX1(i, j) << std::endl;
    std::cout << "MAX(i++,++j) = " << MAX1(i++, ++j) << std::endl;
    std::cout << "i = " << i << ", j = " << j << std::endl;
    printf("PI = %f\n", PI);
    printf(
        "/home/"
        "Documents/\n");
    printf(DOCUMENTS_PATH "\n");
    printf(LONG_STR);
    AREA(3, 4)
    int A() = 0;   // int a = 0;
    int A(1) = 1;  // int a1 = 1;
    std::cout << a << ", " << a1 << std::endl;
    // AREA(3, )
    std::cout << B(2) << std::endl;

    if (true)
        DEBUG_LOG
    else
        printf("else");
    // 展开后的if/else为
    if (true)
        if (false)
            printf("debug!\n");
        else
            printf("else");

    if (true)
        DEBUG_LOG1
    else
        printf("else");

    if (true)
        DEBUG_LOG2;
    else
        printf("else");
}
