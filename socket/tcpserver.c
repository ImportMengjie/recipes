#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define PORT 2048

int main() {
    int servfd = 0, clifd = 0;
    struct sockaddr_in servaddr, cliaddr;
    if ((servfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("create socket error\n");
        exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    if (bind(servfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        printf("bind socket error\n");
        exit(1);
    }
    if (listen(servfd, 1024) < 0) {
        printf("listen socket error\n");
        exit(1);
    }
    printf("server:ip=%s, port=%d\n", inet_ntoa(servaddr.sin_addr),
           ntohs(servaddr.sin_port));

    char buff[2048];
    while (1) {
        socklen_t len = sizeof(cliaddr);
        clifd = accept(servfd, (struct sockaddr*)&cliaddr, &len);
        printf("accept client:ip=%s, port=%d\n", inet_ntoa(cliaddr.sin_addr),
               ntohs(cliaddr.sin_port));
        if (clifd < 0) {
            printf("error accept%d,errorno:%d\n", clifd, errno);
            continue;
        }
        strcpy(buff, "Hello socket!\n");
        strcat(buff, "timestamp:");
        time_t t = time(NULL);
        strcat(buff, ctime(&t));
        send(clifd, buff, strlen(buff) + 1, 0);
        close(clifd);
    }
    close(servfd);
    return 1;
}