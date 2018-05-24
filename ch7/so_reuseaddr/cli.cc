#include "common.h"

int max(int t1, int t2) {
    return t1 > t2 ? t1 : t2;
}

void cliFunc(int sockfd) {
    char buff[1024];
    fd_set rset;
    FD_ZERO(&rset);
    FD_SET(sockfd, &rset);
    FD_SET(fileno(stdin), &rset);
    int maxfd = max(fileno(stdin), sockfd);
    while(1) {
        fd_set nrset = rset;
        int n = select(maxfd + 1, &nrset, NULL, NULL, NULL);
        if(n == -1 && errno == EINTR) continue;
        if(FD_ISSET(sockfd, &nrset)) {
            int len = read(sockfd, buff, 1024);
            write(fileno(stdout), buff, len);
        }
        if(FD_ISSET(fileno(stdin), &nrset)) {
            int len = read(fileno(stdin), buff, 1024);
            if(len == 0) {
                shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(stdin), &rset);
                continue;
            }
            write(sockfd, buff, len);
        }
    }
}

int main(int argc, char **argv) {
    char ip[1024];
    sprintf(ip, "127.0.0.1");
    in_port_t srvPort = htons(29731);
    switch(argc) {
        case 3:
            srvPort = 0;
            while(*argv[2] != '\0') {
                srvPort = srvPort * 10 + *argv[2] - '0';
                argv[2]++;
            }
            srvPort = htons(srvPort);
        case 2:
            strcpy(ip, argv[1]);
    }
    sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = srvPort;
    inet_pton(AF_INET, ip, (void*)&srvaddr.sin_addr.s_addr);
    
    sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(29732);
    inet_pton(AF_INET, ip, (void*)&cliaddr.sin_addr.s_addr); 

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int flag = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void*)&flag, sizeof(flag));
    if(bind(sockfd, (sockaddr*)&cliaddr, sizeof(cliaddr)) == -1) {
        printf("%s\n", strerror(errno));
    }
//    sleep(500);
    printf("wake up!\n");
    if(connect(sockfd, (sockaddr*)&srvaddr, sizeof(srvaddr)) == -1) {
        printf("%s\n", strerror(errno));
    }
    printf("connect success!\n");
    cliFunc(sockfd);
    return 0;
}
