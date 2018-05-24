#include <sys/socket.h>
#include <sys/time.h>
#include <cstdio>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int listenfd;
    sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(12731);
    
    bind(listenfd, (sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, 20);

    while(1) {
        int connfd = accept(listenfd, (sockaddr*)NULL, NULL);
        time_t ticks = time(NULL);
        char buff[1024];
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        int len = strlen(buff);
        for(int i = 0; i < len; i++) {
            write(connfd, buff + i, 1);
            sleep(1);
        }
        close(connfd);
    }

    return 0;
}
