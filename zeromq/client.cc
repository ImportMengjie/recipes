#include <iostream>
#include <stdio.h>
#include <string.h>
#include <zmq.h>

int main() {
  // 创建一个 ZeroMQ 上下文
  void *context = zmq_ctx_new();
  // 创建一个 REQ 套接字
  void *socket = zmq_socket(context, ZMQ_REQ);
  int app_linger = 0;
  int relax = 1;
  int correlate = 1;
  int timeout = 3000;
  zmq_setsockopt(socket, ZMQ_LINGER, &app_linger,
                 sizeof(app_linger)); // important!!!
  zmq_setsockopt(socket, ZMQ_REQ_RELAXED, &relax, sizeof(relax));
  zmq_setsockopt(socket, ZMQ_REQ_CORRELATE, &correlate, sizeof(correlate));
  zmq_setsockopt(socket, ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
  // 连接到服务器端
  zmq_connect(socket, "tcp://localhost:5555");
  char request_msg[] = "0hello";

  for (int request_nbr = 0; request_nbr < 100; ++request_nbr) {
    // 发送请求消息
    // const char *request_msg = "Hello";
    request_msg[0] = '0' + request_nbr;
    zmq_msg_t request;
    zmq_msg_init_size(&request, strlen(request_msg) + 1);
    memcpy(zmq_msg_data(&request), request_msg, strlen(request_msg) + 1);
    std::cout << "before send: " << (char *)zmq_msg_data(&request)
              << ", size: " << zmq_msg_size(&request) << std::endl;
    zmq_msg_send(&request, socket, 0);
    std::cout << "after send: " << (char *)zmq_msg_data(&request)
              << ", size: " << zmq_msg_size(&request) << std::endl;
    static char *s_a = (char *)zmq_msg_data(&request);
    char *a = s_a;
    s_a = (char *)zmq_msg_data(&request);
    zmq_msg_close(&request);
    std::cout << "close send: " << (void *)a << std::endl;

    // 接收回复消息
    zmq_msg_t reply;
    zmq_msg_init(&reply);
    zmq_msg_recv(&reply, socket, 0);
    printf("Received: %s\n", (char *)zmq_msg_data(&reply));
    zmq_msg_close(&reply);
  }

  // 关闭套接字
  zmq_close(socket);
  // 关闭上下文
  zmq_ctx_destroy(context);

  return 0;
}
