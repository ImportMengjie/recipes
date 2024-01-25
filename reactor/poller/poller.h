#pragma once
#include <chrono>
#include <map>
#include <memory>
#include <vector>

namespace mengjie {
namespace reactor {
class EventLoop;
class Channel;
class Poller {
private:
    EventLoop *_loop;

protected:
    std::map<int, Channel *> _channelMap;

public:
    Poller(EventLoop *loop) : _loop(loop) {}

    virtual ~Poller(){};
    virtual std::chrono::system_clock::time_point poll(std::chrono::system_clock::duration timeout,
                                                       std::vector<Channel *> &activeChannels) = 0;
    virtual void updateChannel(Channel *channel) = 0;
    virtual void removeChannel(Channel *channel) = 0;
    virtual bool hasChannel(Channel *channel) = 0;

    static std::unique_ptr<Poller> getDefaultPoller(EventLoop *loop);
};
}  // namespace reactor

}  // namespace mengjie
