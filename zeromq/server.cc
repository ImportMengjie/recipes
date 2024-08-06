#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

int main() {
  std::cout << (10%-3) << std::endl;
    // 创建一个 ZeroMQ 上下文
    void* context = zmq_ctx_new();
    // 创建一个 REP 套接字
    void* socket = zmq_socket(context, ZMQ_REP);
    // 绑定到端口 5555
    zmq_bind(socket, "tcp://*:5555");

    while (true) {
        // 初始化接收消息
        zmq_msg_t request;
        zmq_msg_init(&request);
        zmq_msg_recv(&request, socket, 0);

        // 打印接收到的消息
        printf("Received: %s\n", (char*)zmq_msg_data(&request));
        zmq_msg_close(&request);

        // 模拟处理
        sleep(1);

        // 发送回复消息
        const char* reply_msg = "World";
        zmq_msg_t reply;
        zmq_msg_init_size(&reply, strlen(reply_msg) + 1);
        memcpy(zmq_msg_data(&reply), reply_msg, strlen(reply_msg)+1);
        zmq_msg_send(&reply, socket, 0);
        zmq_msg_close(&reply);
    }

    // 关闭套接字
    zmq_close(socket);
    // 关闭上下文
    zmq_ctx_destroy(context);

    return 0;
}

