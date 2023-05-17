#pragma once
#include <atomic>
#include <thread>
#include <vector>

namespace mengjie {

namespace reactor {
class Channel;
class Poller;

class EventLoop {
   public:
    EventLoop();
    ~EventLoop();

    void loop();

    bool isInLoopThread() { return std::this_thread::get_id() == _thread_ID; }

    void quit() { _quit = true; }

    void updateChannel(Channel* channel);

    void removeChannel(Channel* channel);

    bool hasChannel(Channel* channel);

   private:
    std::thread::id _thread_ID;
    std::unique_ptr<Poller> _poller;
    std::atomic<bool> _quit;
    std::vector<Channel*> _activeChannel;
};
};  // namespace reactor
};  // namespace mengjie
