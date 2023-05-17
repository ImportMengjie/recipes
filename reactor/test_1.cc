#include <memory.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include <iostream>

#include "base/channel.h"
#include "base/eventLoop.h"

using namespace mengjie::reactor;

EventLoop* g_loop;

void timeout(const std::chrono::system_clock::time_point now) {
    std::cout << "timeout on" << now.time_since_epoch().count() << std::endl;
    g_loop->quit();
}

int main() {
    std::cout << "hello" << std::endl;
    EventLoop loop;
    g_loop = &loop;

    int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    Channel channel(&loop, timerfd);
    channel.setReadCallback(timeout);
    struct itimerspec howlong {};
    howlong.it_value.tv_sec = 5;
    std::cout << howlong.it_interval.tv_sec << std::endl;
    timerfd_settime(timerfd, 0, &howlong, NULL);
    loop.loop();
    close(timerfd);
    return 0;
}
