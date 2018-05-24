#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>
#include <cstdio>
#include <sys/errno.h>

int main() {
    sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(29732);
    inet_pton(AF_INET, "127.0.0.1", &srvaddr.sin_addr);
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    connect(sockfd, (sockaddr*)&srvaddr, sizeof(srvaddr));
    
    fd_set rset;
    FD_ZERO(&rset);
    FD_SET(sockfd, &rset);
    FD_SET(fileno(stdin), &rset);
    
    while(1) {
        fd_set nrset = rset;
        int nready = select(sockfd + 1, &nrset, NULL, NULL, NULL);
        if(nready == -1 && errno == EINTR) continue;
        if(FD_ISSET(fileno(stdin), &nrset)) {
            char buff[1024];
            int len = 0;
            if((len = read(fileno(stdin), buff, 1024)) == 0) {
                shutdown(sockfd, SHUT_RD);
                FD_CLR(sockfd, &rset);
            }
            else
                write(sockfd, buff, len);
        }
        if(FD_ISSET(sockfd, &nrset)) {
            char buff[1024];
            int len = read(sockfd, buff, 1023);
            buff[len] = '\0';
            printf("%s", buff);
        }
    }
    return 0;
}
