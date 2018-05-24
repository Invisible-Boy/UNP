#include "common.h"

void chldhandler(int t) {
    int p;
    while((p = waitpid(-1, NULL, WNOHANG)) > 0)
        printf("child %d is handed\n", p);
}

void srvFunc(int connfd) {
    char buff[1024];
    while(1) {
        int len = read(connfd, buff, 1024);
        if(len < 0 && errno == EINTR) continue;
        else if(len > 0)
            write(connfd, buff, len);
        else {
            if(len == 0)
                close(connfd);
            else 
                printf("read error!\n");
            break;
        }
    }
}

int main(int argc, char **argv) {
    in_port_t srvPort = htons(29731);
    if(argc == 2) {
        srvPort = 0;
        while(*argv[1] != '\0') {
            srvPort = srvPort * 10 + *argv[1] - '0';
            argv[1]++;
        }
        srvPort = htons(srvPort);
    }
    sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddr.sin_port = srvPort;

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(listenfd, (sockaddr*)&srvaddr, sizeof(srvaddr));
    if(listen(listenfd, 10) == -1)
        printf("%s\n", strerror(errno));
    signal(SIGCHLD, chldhandler);
    while(1) {
        int connfd = accept(listenfd, NULL, NULL);
        if(connfd > 0) {
            if(fork() == 0) {
                close(listenfd);
                srvFunc(connfd);
                return 0;
            }
            close(connfd);
        }
    }
    return 0;
}
