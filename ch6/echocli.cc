#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <memory.h>
#include <sys/errno.h>
#include <unistd.h>

using namespace std;

const int Nmax = 10;

void clifunc(int connfd) {
    char buff[Nmax], recv[Nmax];
    char *ptr = buff;
    while(ptr != NULL) {
        while((ptr = fgets(buff, sizeof(buff), stdin)) != NULL) {
            size_t len = strlen(buff);
            while(write(connfd, buff, len + 1) < 0 && errno == EINTR);
            if(buff[len - 1] == '\n')
                break;
        }
    if(ptr == NULL) close(connfd);
    again:
        int n;
        while((n = read(connfd, recv, sizeof(recv))) > 0) {
            size_t len = strlen(recv);
            printf("%s", recv);
            bool flag = false;
            for(size_t i = 0; i < len; i++)
                if(recv[i] == '\n') {
                    flag = true;
                    break;
                }
            if(flag) break;
        }
        if(n < 0 && errno == EINTR) goto again;
    }
}

int main() {
    sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(29732);
    char ip[] = "127.0.0.1";
    inet_pton(AF_INET, ip, &srvaddr.sin_addr);
    
    int connfd = socket(AF_INET, SOCK_STREAM, NULL);
    connect(connfd, (sockaddr*)&srvaddr, sizeof(srvaddr));
    clifunc(connfd);
    return 0;
}
