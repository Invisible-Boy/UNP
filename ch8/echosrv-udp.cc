#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory.h>
#include <iostream>


int main() {
    sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(29731);
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int srvfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(bind(srvfd, (sockaddr*)&srvaddr, sizeof(srvaddr)) == -1) {
        std::cout << "bind error" << std::endl;
        return -1;
    }
    sockaddr_in recvaddr;
    socklen_t len = sizeof(recvaddr);
    char buff[1024];
    const char quit[] = "bye";
    while(1) {
        int n = recvfrom(srvfd, buff, sizeof(buff), 0, (sockaddr*)&recvaddr, &len);
        sendto(srvfd, buff, n, 0, (sockaddr*)&recvaddr, sizeof(recvaddr));
        if(memcmp(buff, quit, 3) == 0) return 0;
    }
    return 0;
}
