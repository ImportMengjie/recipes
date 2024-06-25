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
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

#define LOG_ERROR std::cerr << std::endl
#define LOG_INFO std::cout << std::endl

bool check_file_exist(const std::string &file_path) {
    std::ifstream file(file_path);
    return file.good();
}

class LogStringStream : public std::ostringstream {
public:
    ~LogStringStream() {
        auto log_str = this->str();
        if (log_str.size()) {
            LOG_INFO << log_str;
        }
    }

    void flush_log() {
        auto log_str = this->str();
        if (log_str.size()) {
            LOG_INFO << log_str;
            this->str("");
        }
    }
};
class DeferFunction {
public:
    explicit DeferFunction(const std::function<void()> &defer_func) : defer_func_(defer_func) {}

    ~DeferFunction() { defer_func_(); }

private:
    std::function<void()> defer_func_;
};

class ShellProcess {
public:
    ShellProcess() = default;

    ShellProcess(const std::string &cmd) : pid_(exec(cmd)) {}

    ShellProcess(ShellProcess &&lhs) noexcept : pid_(lhs.pid_) { lhs.pid_ = -1; }

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
        if (pid_ > 0) {
            LogStringStream log_ss;
            log_ss << __PRETTY_FUNCTION__ << ", pid: " << pid_;
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
        log_ss << __PRETTY_FUNCTION__ << ", pid: " << pid_;
        if (pid_ > 0) {
            int status = -1;
            int waitpid_ret = ::waitpid(pid_, &status, 0);
            pid_ = -1;
            log_ss << ", status: " << status << ", exited: " << (WIFEXITED(status) ? WEXITSTATUS(status) : -1)
                   << ", signal: " << (WIFSIGNALED(status) ? WTERMSIG(status) : -1) << ", waitpid_ret: " << waitpid_ret;
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
        log_ss << __PRETTY_FUNCTION__ << ", cmd: " << cmd;
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
            log_ss << ", pid: " << pid << ", setpgid ret: " << ::setpgid(pid, pid);
        }
        return pid;
    }

    int waitNoBlock() {
        DeferFunction defer_func([this] { this->pid_ = -1; });
        LogStringStream log_ss;
        log_ss << __PRETTY_FUNCTION__ << ", pid: " << pid_;
        if (pid_ > 0) {
            static const int max_retry_times = 15;
            int status = -1;
            int waitpid_ret = 0;
            int retry_times = max_retry_times;
            do {
                waitpid_ret = ::waitpid(pid_, &status, WNOHANG);
                if (max_retry_times - retry_times < 5)
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                else
                    std::this_thread::yield();

            } while (--retry_times > 0 && ((waitpid_ret == 0) || (waitpid_ret == -1 && errno == EINTR) ||
                                           (waitpid_ret != -1 && !WIFEXITED(status) && !WIFSIGNALED(status))));
            log_ss << ", retry_times: " << retry_times + 1 << ", status: " << status
                   << ", exited: " << (WIFEXITED(status) ? WEXITSTATUS(status) : -1)
                   << ", signal: " << (WIFSIGNALED(status) ? WTERMSIG(status) : -1) << ", waitpid_ret: " << waitpid_ret;
            return status;
        }
        return -1;
    }

    pid_t pid_ = -1;
};

class VoicePlayer {
public:
    void playContinuous(const std::string &voice_path) {
        LogStringStream log_ss;
        log_ss << __PRETTY_FUNCTION__ << ", path: " << voice_path;
        if (check_file_exist(voice_path)) {
            std::lock_guard<std::mutex> lk_shell_process(shell_process_mutex_);
            shell_process_ = ShellProcess("while [ $PPID -eq $(cut -d ' ' -f 4 /proc/$$/stat) ]; do aplay " + voice_path + "; done");
            log_ss << ", pid: " << shell_process_.pid();
            cur_voice_path = voice_path;
        } else {
            log_ss << ", not exist";
        }
    }

    void playOnce(const std::string &voice_path) {
        LogStringStream log_ss;
        log_ss << __PRETTY_FUNCTION__ << ", path: " << voice_path;
        if (check_file_exist(voice_path)) {
            std::lock_guard<std::mutex> lk_shell_process(shell_process_mutex_);
            shell_process_ = ShellProcess("aplay " + voice_path);
            log_ss << ", pid: " << shell_process_.pid();
            cur_voice_path = voice_path;
        } else {
            log_ss << ", not exist";
        }
    }

    void playOnceBlock(const std::string &voice_path) {
        LogStringStream log_ss;
        log_ss << __PRETTY_FUNCTION__ << ", path: " << voice_path;
        if (check_file_exist(voice_path)) {
            pid_t pid_voice = -1;
            {
                std::lock_guard<std::mutex> lk_shell_process(shell_process_mutex_);
                shell_process_ = ShellProcess("aplay " + voice_path);
                pid_voice = shell_process_.pid();
                log_ss << ", pid: " << pid_voice;
                cur_voice_path = voice_path;
            }
            log_ss.flush_log();
            if (pid_voice > 0) {
                int status = -1;
                int waitpid_ret = ::waitpid(pid_voice, &status, 0);  // waitpid阻塞时不持有锁, 其它线程可调用stop提前结束
                log_ss << "waitpid, status: " << status << ", exited: " << (WIFEXITED(status) ? WEXITSTATUS(status) : -1)
                       << ", signal: " << (WIFSIGNALED(status) ? WTERMSIG(status) : -1) << ", waitpid_ret: " << waitpid_ret;
            }
            {
                std::lock_guard<std::mutex> lk_shell_process(shell_process_mutex_);
                log_ss << "cur_pid: " << shell_process_.pid() << ", pid_voice: " << pid_voice;
                if (shell_process_.pid() == pid_voice) log_ss << ", terminate: " << shell_process_.terminate();
                cur_voice_path = "";
            }
        } else {
            log_ss << ", not exist";
        }
    }

    void stop(const std::string &voice_path) {
        LogStringStream log_ss;
        log_ss << __PRETTY_FUNCTION__ << ", voice_path: " << voice_path;
        std::lock_guard<std::mutex> lk_shell_process(shell_process_mutex_);
        log_ss << ", cur_voice: " << cur_voice_path;
        if (voice_path == cur_voice_path) {
            log_ss << ", terminate: " << shell_process_.terminate();
            cur_voice_path = "";
        }
    }

    void stop() {
        LogStringStream log_ss;
        log_ss << __PRETTY_FUNCTION__;
        std::lock_guard<std::mutex> lk_shell_process(shell_process_mutex_);
        log_ss << ", terminate: " << shell_process_.terminate();
        cur_voice_path = "";
    }

private:
    std::mutex shell_process_mutex_;
    ShellProcess shell_process_;
    std::string cur_voice_path;
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

#define WAIT_LINE                                                \
    std::cout << std::endl << "line: " << __LINE__ << std::endl; \
    std::cin >> tmp;

int main() {
    int tmp = 0;
    std::string voice_long{"/home/mengjie/Music/security_alarm.wav"};
    std::string voice_short{"/home/mengjie/Music/beep_warning.wav"};
    VoicePlayer p;
    p.playContinuous(voice_short);
    WAIT_LINE
    p.stop();
    WAIT_LINE
    p.playOnce(voice_long);
    WAIT_LINE
    p.stop(voice_short);
    WAIT_LINE
    p.stop(voice_long);
    WAIT_LINE
    p.playOnceBlock(voice_short);
    WAIT_LINE
    std::thread([&] {
        sleep(5);
        p.stop(voice_long);
    }).detach();
    p.playOnceBlock(voice_long);
    WAIT_LINE

    // p1.terminate();
    return 0;
}
