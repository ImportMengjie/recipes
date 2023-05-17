#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define PORT 2048

int main() {
    srand(time(NULL));
    int clifd = 0;
    int cliport = rand() % (65536 - 1024) + 1024;
    char* server_addr = "127.0.0.1";
    struct sockaddr_in servaddr, cliaddr;
    char* buff[1024];
    socklen_t addr_len = sizeof(servaddr);

    if ((clifd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("create socket error\n");
        exit(1);
    }
    memset(&cliaddr, 0, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(cliport);
    cliaddr.sin_addr.s_addr = htons(INADDR_ANY);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(server_addr);
    servaddr.sin_port = htons(PORT);

    if (bind(clifd, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) < 0) {
        printf("bind socket error\n");
        exit(1);
    }
    printf("client:ip=%s, port=%d\n", inet_ntoa(cliaddr.sin_addr),
           ntohs(cliaddr.sin_port));
    printf("server:ip=%s, port=%d\n", inet_ntoa(servaddr.sin_addr),
           ntohs(servaddr.sin_port));
    if (connect(clifd, (struct sockaddr*)&servaddr, addr_len) < 0) {
        printf("connect socket error\n");
        exit(1);
    }
    int length = recv(clifd, buff, sizeof(buff), 0);
    if (length < 0) {
        printf("error lenght<0\n");
    }
    printf("from server:%s\n", buff);
    return 0;
}