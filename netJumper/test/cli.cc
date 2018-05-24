#include <cstdio>
#include <memory.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;

void print(sockaddr_in &addr) {
    char ip[1024];
    inet_ntop(AF_INET, (void*)&addr.sin_addr.s_addr, ip, 1024);
    printf("%s %d\n", ip, ntohs(addr.sin_port));
}

int main(int argc, char **argv) {
    sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_len = sizeof(srvaddr);
    inet_pton(AF_INET, argv[1], (void*)&srvaddr.sin_addr.s_addr);
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int n = 10;
    while(n--) {
        char buff[] = "111";
        srvaddr.sin_port = htons(29732);
        sendto(sockfd, buff, sizeof(buff), 0, (sockaddr*)&srvaddr, sizeof(srvaddr));
        srvaddr.sin_port = htons(29750);
        sendto(sockfd, buff, sizeof(buff), 0, (sockaddr*)&srvaddr, sizeof(srvaddr));
    }
    return 0;
}
