#include <asm-generic/errno-base.h>
#include <cerrno>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <functional>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

#define LOG_ERROR std::cerr
#define LOG_INFO std::cout

class LogStringStream : public std::ostringstream {

public:
  LogStringStream() : defer_func_([] {}) {}
  explicit LogStringStream(const std::function<void()> &defer_func)
      : defer_func_(defer_func) {}
  ~LogStringStream() {
    defer_func_();
    LOG_INFO << this->str();
  }

private:
  std::function<void()> defer_func_;
};

class ShellProcess {

public:
  ShellProcess() = default;

  ShellProcess(const std::string &cmd) : pid_(exec(cmd)) {}

  ShellProcess(ShellProcess &&lhs) noexcept {
    lhs.pid_ = pid_;
    pid_ = -1;
  }

  ShellProcess &operator=(ShellProcess &&lhs) noexcept {
    if (this != &lhs) {
      terminate();
      this->pid_ = lhs.pid_;
      lhs.pid_ = -1;
    }
    return *this;
  }

  ShellProcess(const ShellProcess &) = delete;
  ShellProcess &operator=(const ShellProcess &) = delete;

  int terminate() {
    LogStringStream log_ss;
    log_ss << __func__ << ", pid: " << pid_;
    if (pid_ > 0) {
      int killpg_ret = ::killpg(pid_, SIGKILL);
      log_ss << ", killpg_ret: " << killpg_ret;
      if (killpg_ret < 0) {
        log_ss << ", error: " << strerror(errno);
      }

      return waitNoBlock();
    }
    return -1;
  }

  int wait() {
    LogStringStream log_ss;
    log_ss << __func__ << ", pid: " << pid_;
    if (pid_ > 0) {
      int status = -1;
      int waitpid_ret = ::waitpid(pid_, &status, 0);
      pid_ = -1;
      log_ss << ", waitpid_ret: " << waitpid_ret << ", status: " << status;
      if (waitpid_ret < 0) {
        log_ss << ", error: " << strerror(errno);
        return -1;
      }
      return status;
    }
    return -1;
  }

  ~ShellProcess() { terminate(); }

  pid_t id() const;

private:
  pid_t exec(const std::string &cmd) {
    pid_t pid = fork();
    if (pid == 0) {
      ::setpgid(0, 0);
      ::execl("/bin/sh", "sh", "-c", cmd.c_str(), nullptr);
      LOG_ERROR << "execl failure!";
      _exit(EXIT_FAILURE);
    } else if (pid < 0) {
      LOG_ERROR << "fork failed: " << pid;
    } else {
      setpgid(pid, pid);
    }
    return pid;
  }

  int waitNoBlock() {
    LogStringStream log_ss([this] { this->pid_ = -1; });
    log_ss << __func__ << ", pid: " << pid_;
    if (pid_ > 0) {
      static const int max_retry_times = 5;
      int status = -1;
      int waitpid_ret = 0;
      int retry_times = max_retry_times;
      do {
        waitpid_ret = ::waitpid(pid_, &status, WNOHANG);
        if (retry_times == max_retry_times)
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
        else
          std::this_thread::yield();

      } while (
          --retry_times > 0 &&
          ((waitpid_ret == 0) || (waitpid_ret == -1 && errno == EINTR) ||
           (waitpid_ret != -1 && !WIFEXITED(status) && !WIFSIGNALED(status))));
      log_ss << ", retry_times: " << retry_times << ", status: " << status
             << ", waitpid_ret: " << waitpid_ret;
      return status;
    }
    return -1;
  }

  pid_t pid_ = -1;
};

int main() {}
