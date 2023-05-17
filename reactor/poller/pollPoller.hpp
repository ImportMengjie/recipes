#pragma once
#include <assert.h>
#include <errno.h>
#include <poll.h>

#include <vector>
#include <iostream>

#include "channel.h"
#include "eventLoop.h"
#include "poller.h"

namespace mengjie {
namespace reactor {
class pollPoller : public Poller {
   private:
    std::vector<struct pollfd> _pollfds;

   public:
    virtual std::chrono::system_clock::time_point poll(
        std::chrono::system_clock::duration timeout,
        std::vector<Channel*>& activeChannels) override {
        activeChannels.clear();
        int timeout_ = static_cast<int>(
            std::chrono::duration_cast<std::chrono::milliseconds>(timeout)
                .count());
        int numEvents = ::poll(_pollfds.data(), _pollfds.size(), timeout_);
        auto now = std::chrono::system_clock::now();
        int saveError = errno;
        if (numEvents > 0) {
            for (size_t i = 0; i < _pollfds.size(); i++) {
                if (_pollfds[i].revents > 0) {
                    _channelMap[_pollfds[i].fd]->setREvent(_pollfds[i].revents);
                    activeChannels.push_back(_channelMap[_pollfds[i].fd]);
                }
            }
        } else if (numEvents < 0) {
            std::cerr<<"error in poll:"<<saveError<<std::endl;
            if (saveError != EINTR) {
                errno = saveError;
            }
        } else {
            std::cout<<"zero numEvents"<<std::endl;
        }
        return now;
    }
    virtual void updateChannel(Channel* channel) override {
        if (channel->getIndex() >= 0) {
            assert(_channelMap.count(channel->fd()));
            assert(0 <= channel->getIndex() &&
                   channel->getIndex() < _pollfds.size());
            auto& pfd = _pollfds[channel->getIndex()];
            pfd.events = channel->getEvent();
        } else {
            assert(_channelMap.count(channel->fd()) == 0);
            struct pollfd pfd;
            pfd.fd = channel->fd();
            pfd.events = channel->getEvent();
            pfd.revents = 0;
            _pollfds.push_back(pfd);
            channel->setIndex(_pollfds.size() - 1);
            _channelMap[pfd.fd] = channel;
        }
    }
    virtual void removeChannel(Channel* channel) override {
        if (_channelMap.count(channel->fd())) _channelMap.erase(channel->fd());
        if (0 <= channel->getIndex() && channel->getIndex() < _pollfds.size())
            _pollfds.erase(_pollfds.begin() + channel->getIndex());
    }
    virtual bool hasChannel(Channel* channel) override {
        return _channelMap.count(channel->fd());
    }

    pollPoller(EventLoop* loop) : Poller(loop) {}
    ~pollPoller() {}
};

};  // namespace reactor
};  // namespace mengjie
