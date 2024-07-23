#include <fcntl.h>
#include <linux/videodev2.h>
#include <memory.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <unistd.h>

#include <iostream>
#include <string>

#define BUFFER_COUNT 4
#define WIDTH 640
#define HEIGHT 480
#define LOG(level) std::cout << std::endl << #level << " " << __FILE__ << ":" << __LINE__ << " "
#define LOG_INFO LOG(INFO)
#define LOG_ERROR LOG(ERROR)

#define errExit(stmt)                                                                      \
    {                                                                                      \
        if ((stmt) < 0) {                                                                  \
            LOG_ERROR << #stmt << ", errno: " << errno << ", errstr: " << strerror(errno); \
            exit(1);                                                                       \
        }                                                                                  \
    }

struct buffer {
    void *start;
    uint length;
} buffers[BUFFER_COUNT];

int main() {
    int vfd = -1;
    errExit(vfd = open("/dev/video0", O_RDWR));
    v4l2_capability cap{0};
    errExit(ioctl(vfd, VIDIOC_QUERYCAP, &cap));
    LOG_INFO << "driver: " << cap.driver << " , card: " << cap.card << " , bus: " << cap.bus_info << " , version: " << cap.version
             << " , cap: " << cap.capabilities;
    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        LOG_ERROR << "not capture device";
        return -1;
    }

    struct v4l2_fmtdesc fmtdesc;
    fmtdesc.index = 0;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    while (ioctl(vfd, VIDIOC_ENUM_FMT, &fmtdesc) != -1) {
        LOG_INFO << "index: " << fmtdesc.index << ", desc: " << fmtdesc.description;
        ++fmtdesc.index;
    }

    struct v4l2_format fmt {};
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = WIDTH;
    fmt.fmt.pix.height = HEIGHT;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_JPEG;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;
    errExit(ioctl(vfd, VIDIOC_S_FMT, &fmt));

    struct v4l2_requestbuffers req;
    req.count = BUFFER_COUNT;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    errExit(ioctl(vfd, VIDIOC_REQBUFS, &req));

    struct v4l2_buffer buf {};
    for (int n_buffer = 0; n_buffer < req.count; ++n_buffer) {
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffer;
        errExit(ioctl(vfd, VIDIOC_QUERYBUF, &buf));
        LOG_INFO << "length: " << buf.length << ", offset: " << buf.m.offset << ", fd: " << buf.m.fd << ", index: " << buf.index;
        buffers[n_buffer].length = buf.length;
        buffers[n_buffer].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, vfd, buf.m.offset);
        if (MAP_FAILED == buffers[n_buffer].start) {
            perror("mmap");
            continue;
        }
        errExit(ioctl(vfd, VIDIOC_QBUF, &buf));
    }

    errExit(ioctl(vfd, VIDIOC_STREAMON, &buf.type));

    // fd_set fds;
    // FD_ZERO(&fds);
    // FD_SET(vfd, &fds);
    // struct timeval tv {};
    // tv.tv_sec = 2;
    // errExit(select(vfd + 1, &fds, NULL, NULL, &tv));
    struct v4l2_buffer capture_buf {};
    capture_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    capture_buf.memory = V4L2_MEMORY_MMAP;
    int img_cnt = 0;
    for (int i = 0; img_cnt < BUFFER_COUNT * 2; ++i) {
        errExit(ioctl(vfd, VIDIOC_DQBUF, &capture_buf));
        LOG_INFO << "idx: " << i << ", bytesused: " << capture_buf.bytesused;
        if (0 != capture_buf.bytesused && 0 == i % 10) {
            ++img_cnt;
            int imgfd = open((std::string("capture_") + std::to_string(i) + ".jpg").c_str(), O_WRONLY | O_CREAT, 0666);
            errExit(imgfd);
            errExit(write(imgfd, buffers[capture_buf.index].start, capture_buf.bytesused));
            errExit(close(imgfd));
        }

        if (0 != ioctl(vfd, VIDIOC_QBUF, &capture_buf)) {
            perror("VIDIOC_QBUF");
            return 1;
        }
    }
    errExit(ioctl(vfd, VIDIOC_STREAMOFF, &buf.type));
    for (int i = 0; i < BUFFER_COUNT; ++i) {
        munmap(buffers[i].start, buffers[i].length);
    }
    close(vfd);
}
