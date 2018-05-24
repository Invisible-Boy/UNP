#include <cstdio>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <memory.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("input error!\n");
        return 0;
    }
    sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(12731);
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0) {
        printf("inet_pton error for %s\n", argv[1]);
        return 0;
    }
    int buffsize, mss;
    socklen_t len = sizeof(buffsize);
    getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &buffsize, &len);
    getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &mss, &len);
    printf("before connect cli rcvbuffsize is %d, mss is %d\n", buffsize, mss);
    if(connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        printf("connect error!\n");
    else {
        getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &buffsize, &len);
        getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &mss, &len);
        printf("after connect cli rcvbuffsize is %d, mss is %d\n", buffsize, mss);
        char buff[1024];
        int n;
        while((n = read(sockfd, buff, 1024)) > 0) {
            buff[n] = NULL;
            printf("%s", buff);
        }
        close(sockfd);
    }
}
