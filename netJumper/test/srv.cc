/**
 * Title:   NAT type test
 * Atuhro:  InvisibleMan
 * Date:    2018/4/15
 * Remark:  通过这个测试，移动网络的Nat似乎是最严格的对称型NAT。
 */
#include <cstdio>
#include <memory.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;

void print(sockaddr_in &addr) {
    char ip[10];
    inet_ntop(AF_INET, (void*)&addr.sin_addr.s_addr, ip, 10);
    printf("%s %d\n", ip, ntohs(addr.sin_port));
}

short iton(char *buff) {
    int ans = 0;
    while(*buff != '\0') {
        ans = ans * 10 + *buff - '0';
        buff++;
    }
    return ans;
}

int main(int argc, char **argv) {
    sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_len = sizeof(srvaddr);
    srvaddr.sin_port = htons(iton(argv[1]));
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sockfd, (sockaddr*)&srvaddr, sizeof(srvaddr));
    int sendfd = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in sendaddr;
    memset(&sendaddr, 0, sizeof(sendaddr));
    sendaddr.sin_family = AF_INET;
    sendaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sendaddr.sin_port = htons(29750);
    bind(sendfd, (sockaddr*)&sendaddr, sizeof(sendaddr));
    while(1) {
        char buff[1024];
        sockaddr_in recvaddr;
        socklen_t len = sizeof(recvaddr);
        recvfrom(sockfd, buff, 1024, 0, (sockaddr*)&recvaddr, &len);
        print(recvaddr);
        recvfrom(sendfd, buff, 1024, 0, (sockaddr*)&recvaddr, &len);
        print(recvaddr);
    }
    return 0;
}
