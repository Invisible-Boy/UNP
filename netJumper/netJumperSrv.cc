#include <cstdio>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <memory.h>
#include <sys/select.h>
#include <string>
#include <errno.h>

using namespace std;

void keepLive(int sockfd, sockaddr_in peerAddr, timeval timeout, string msg) {
    if(fork() == 0) {
        while(1) {
            select(0, NULL, NULL, NULL, &timeout);
            sendto(sockfd, msg.c_str(), msg.length(), 0, (sockaddr*)&peerAddr, sizeof(peerAddr));
        }
    }
    return;
}

int iton(char *buff) {
    int ans = 0;
    while(*buff != '\0') {
        ans = ans * 10 + *buff - '0';
        buff++;
    }
    return ans;
}

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("input error!\n");
        return -1;
    }
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in sendaddr;
    memset(&sendaddr, 0, sizeof(sendaddr));
    sendaddr.sin_family = AF_INET;
    sendaddr.sin_port = htons(27001);
    int fff = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &fff, sizeof(fff));
    bind(sockfd, (sockaddr*)&sendaddr, sizeof(sendaddr));
    sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_len = sizeof(srvaddr);
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(29732);
    inet_pton(AF_INET, argv[1], &srvaddr.sin_addr.s_addr);
    fd_set rset;
    FD_ZERO(&rset);
    FD_SET(sockfd, &rset);
    timeval timeout = {5, 0};
    int p;
    if((p = fork()) == 0) {
        string msg("hello Lily");
        keepLive(sockfd, srvaddr, timeout, msg);
        return 0;
    }
    waitpid(p, NULL, 0);
    sockaddr_in peeraddr;
    socklen_t len = sizeof(peeraddr);
    string name;
    bool flag = false, flag2 = false;
    while(1) {
        fd_set nrset = rset;
        int n = select(sockfd + 1, &nrset, NULL, NULL, NULL);
        if(n == -1 && errno == EINTR) continue;
        if(FD_ISSET(sockfd, &nrset)) {
            sockaddr_in recvaddr;
            len = sizeof(recvaddr);
            char buff[2048];
            int n = recvfrom(sockfd, buff, 2047, 0, (sockaddr*)&recvaddr, &len);
            buff[n] = '\0';
            if(!flag && len == sizeof(srvaddr) && memcmp(&recvaddr, &srvaddr, len) == 0) {
                printf("Find srv:%s\n", buff);
                memset(&peeraddr, 0, sizeof(peeraddr));
                peeraddr.sin_family = AF_INET;
                char nm[1024], ip[1024], port[1024];
                sscanf(buff, "%s%s%s", nm, ip, port);
                peeraddr.sin_port = htons(iton(port));
                name = nm;
                inet_pton(AF_INET, ip, &peeraddr.sin_addr.s_addr);
                peeraddr.sin_len = sizeof(peeraddr);
                if(!flag2 && (p = fork()) == 0) {
                    string msg("hello Yvan");
                    keepLive(sockfd, peeraddr, timeout, msg);
                    return 0;
                }
                flag2 = true;
                waitpid(p, NULL, 0);
                printf("we are connecting %s.\n", name.c_str());
            }
            else if(len == sizeof(peeraddr) && memcmp(&recvaddr, &peeraddr, len) == 0) {
                if(!flag) {
                    printf("connecting success.\n");
                    flag = true;
                }
                char hello[] = "hello Lily";
                if(strcmp(buff, hello) == 0) continue;
                sendto(sockfd, buff, n, 0, (sockaddr*)&peeraddr, sizeof(peeraddr));
            }
        }
    }
    return 0;
}
