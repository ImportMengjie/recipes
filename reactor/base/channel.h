#pragma once
#include <poll.h>

#include <chrono>
#include <functional>

namespace mengjie {
namespace reactor {

class EventLoop;

class Channel {
   private:
    EventLoop* _loop;
    int _fd;
    int _index;
    short _event = 0;
    short _revent = 0;
    std::function<void(const std::chrono::system_clock::time_point)> _readcb;
    std::function<void()> _writecb;
    std::function<void()> _closecb;
    std::function<void()> _errorcb;

   public:
    Channel(EventLoop* loop, int fd) : _loop(loop), _fd(fd), _index(-1) {}
    Channel(const Channel&) = delete;
    Channel(const Channel&&) = delete;
    Channel& operator=(const Channel&) = delete;

    void handleEvent(const std::chrono::system_clock::time_point timepoint) {
        if (_revent & POLLHUP && (!_revent & POLLIN))
            if (_closecb) _closecb();
        if (_revent & (POLLERR | POLLNVAL))
            if (_errorcb) _errorcb();
        if (_revent & (POLLIN | POLLPRI | POLLRDHUP))
            if (_readcb) _readcb(timepoint);
        if (_revent & POLLOUT)
            if (_writecb) _writecb();
    }

    void update();

    void setReadCallback(
        std::function<void(const std::chrono::system_clock::time_point)> cb) {
        _readcb = std::move(cb);
        if (_readcb)
            _event |= POLLIN | POLLPRI;
        else
            _event &= ~(POLLIN | POLLPRI);
        update();
    }
    void setWriteCallback(std::function<void()> cb) {
        _writecb = std::move(cb);
        if (_writecb)
            _event |= POLLOUT;
        else
            _event &= ~POLLOUT;
        update();
    }
    void setCloseCallback(std::function<void()> cb) {
        _closecb = std::move(cb);
    }
    void setErrorCallback(std::function<void()> cb) {
        _errorcb = std::move(cb);
    }

    short getEvent() { return _event; }
    void setREvent(const short& revent) { _revent = revent; }

    int fd() const { return _fd; }
    int getIndex() const { return _index; }
    void setIndex(int index) { _index = index; }
    EventLoop* loop() const { return _loop; }
};
}  // namespace reactor

}  // namespace mengjie
