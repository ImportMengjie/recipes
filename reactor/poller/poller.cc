#include "poller.h"

#include "pollPoller.hpp"

using namespace mengjie::reactor;

std::unique_ptr<Poller> mengjie::reactor::Poller::getDefaultPoller(
    EventLoop* loop) {
    return std::unique_ptr<Poller>(new pollPoller(loop));
}
