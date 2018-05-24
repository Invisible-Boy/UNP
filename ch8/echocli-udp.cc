#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <memory.h>

using namespace std;

int main(int argc, char **argv) {
    if(argc != 2) {
        cout << "input error! argc == " << argc << endl;
        return -1;
    }
    sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
#ifndef debug
    srvaddr.sin_len = sizeof(srvaddr);
#endif
    srvaddr.sin_port = htons(29731);
    srvaddr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &srvaddr.sin_addr.s_addr);
    int connfd = socket(AF_INET, SOCK_DGRAM, 0);
    char buff[1024];
    const char quit[] = "bye";
    while(1) {
        int n = read(fileno(stdin), buff, sizeof(buff) - 1);
        sendto(connfd, buff, n, 0, (sockaddr*)&srvaddr, sizeof(srvaddr));
        sockaddr_in recvaddr;
        socklen_t len = sizeof(recvaddr);
        n = recvfrom(connfd, buff, sizeof(buff), 0, (sockaddr*)&recvaddr, &len);
        if(len == sizeof(srvaddr) && memcmp(&srvaddr, &recvaddr, len) == 0) {
            buff[n] = '\0';
            cout << buff;
        }
        if(memcmp(quit, buff, 3) == 0)
            return 0;
    }
}
