#include <asm-generic/errno-base.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cerrno>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#define LOG_ERROR std::cerr << std::endl
#define LOG_INFO std::cout << std::endl

class LogStringStream : public std::ostringstream {
   public:
    ~LogStringStream() { LOG_INFO << this->str(); }
};

class DeferFunction {
   public:
    explicit DeferFunction(const std::function<void()> &defer_func)
        : defer_func_(defer_func) {}

    ~DeferFunction() { defer_func_(); }

   private:
    std::function<void()> defer_func_;
};

class ShellProcess {
   public:
    ShellProcess() = default;

    ShellProcess(const std::string &cmd) : pid_(exec(cmd)) {}

    ShellProcess(ShellProcess &&lhs) noexcept : pid_(lhs.pid_) {
        lhs.pid_ = -1;
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
            std::this_thread::yield();
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
            log_ss << ", status: " << status << ", exited: "
                   << (WIFEXITED(status) ? WEXITSTATUS(status) : -1)
                   << ", signal: "
                   << (WIFSIGNALED(status) ? WTERMSIG(status) : -1)
                   << ", waitpid_ret: " << waitpid_ret;
            if (waitpid_ret < 0) {
                log_ss << ", error: " << strerror(errno);
                return -1;
            }
            return status;
        }
        return -1;
    }

    ~ShellProcess() { terminate(); }

    pid_t pid() const { return pid_; };

   private:
    pid_t exec(const std::string &cmd) {
        LogStringStream log_ss;
        log_ss << __func__ << ", cmd: " << cmd;
        if (cmd.empty()) return -1;
        pid_t pid = fork();
        if (pid == 0) {
            ::setpgid(0, 0);
            ::execl("/bin/sh", "sh", "-c", cmd.c_str(), nullptr);
            LOG_ERROR << "execl failure!";
            _exit(EXIT_FAILURE);
        } else if (pid < 0) {
            LOG_ERROR << "fork failed: " << pid;
        } else {
            log_ss << ", pid: " << pid
                   << ", setpgid ret: " << ::setpgid(pid, pid);
        }
        return pid;
    }

    int waitNoBlock() {
        DeferFunction defer_func([this] { this->pid_ = -1; });
        LogStringStream log_ss;
        log_ss << __func__ << ", pid: " << pid_;
        if (pid_ > 0) {
            static const int max_retry_times = 5;
            int status = -1;
            int waitpid_ret = 0;
            int retry_times = max_retry_times;
            do {
                waitpid_ret = ::waitpid(pid_, &status, WNOHANG);
                if (retry_times == max_retry_times)
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                else
                    std::this_thread::yield();

            } while (--retry_times > 0 &&
                     ((waitpid_ret == 0) ||
                      (waitpid_ret == -1 && errno == EINTR) ||
                      (waitpid_ret != -1 && !WIFEXITED(status) &&
                       !WIFSIGNALED(status))));
            log_ss << ", retry_times: " << retry_times + 1
                   << ", status: " << status << ", exited: "
                   << (WIFEXITED(status) ? WEXITSTATUS(status) : -1)
                   << ", signal: "
                   << (WIFSIGNALED(status) ? WTERMSIG(status) : -1)
                   << ", waitpid_ret: " << waitpid_ret;
            return status;
        }
        return -1;
    }

    pid_t pid_ = -1;
};

bool isProcessRunning(pid_t pid) {
    LogStringStream log_ss;
    bool ret = (kill(pid, 0) == 0);
    log_ss << __func__ << ", ret: " << ret;
    return ret;
}

void func() {
    ShellProcess p1("sleep 100");
    isProcessRunning(p1.pid());
    p1.terminate();
    isProcessRunning(p1.pid());

    ShellProcess p2("true");
    isProcessRunning(p2.pid());
    int tmp = 0;
    std::cin >> tmp;
}

int main() {
    func();
    // p1.terminate();
    int tmp = 0;
    std::cin >> tmp;
    return 0;
}
