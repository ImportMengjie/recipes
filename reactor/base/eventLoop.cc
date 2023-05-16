#include "eventLoop.h"

#include <assert.h>

#include "channel.h"
#include "poller.h"

const int PollTimeMs = 10000;

mengjie::reactor::EventLoop::EventLoop()
    : _thread_ID(std::this_thread::get_id()),
      _poller(Poller::getDefaultPoller(this)),
      _quit(false) {}
mengjie::reactor::EventLoop::~EventLoop() {}

void mengjie::reactor::EventLoop::loop() {
    assert(isInLoopThread());
    _quit.store(false);
    while (!_quit) {
        auto time_point = _poller->poll(std::chrono::milliseconds(PollTimeMs),
                                        _activeChannel);
        for (auto& channel : _activeChannel) {
            channel->handleEvent(time_point);
        }
    }
}

void mengjie::reactor::EventLoop::updateChannel(Channel* channel) {
    assert(channel->loop() == this);
    assert(isInLoopThread());
    _poller->updateChannel(channel);
}

void mengjie::reactor::EventLoop::removeChannel(Channel* channel) {
    assert(isInLoopThread());
    _poller->removeChannel(channel);
}

bool mengjie::reactor::EventLoop::hasChannel(Channel* channel) {
    assert(isInLoopThread());
    return _poller->hasChannel(channel);
}
