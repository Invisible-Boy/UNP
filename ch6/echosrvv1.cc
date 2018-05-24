#include <cstdio>
#include <memory.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <algorithm>
#include <errno.h>

using namespace std;

inline int search(int aim, int buff[], int size) {
    int i = 0;
    while(i < size && buff[i] != aim) i++;
    return i;
}

int main() {
    sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(29732);
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int connetfd[FD_SETSIZE], maxfd = 0;
    memset(connetfd, -1, sizeof(connetfd));
    
    fd_set rset;
    FD_ZERO(&rset);
    
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    int index = search(-1, connetfd, FD_SETSIZE);
    if(index < FD_SETSIZE) {
        connetfd[index] = listenfd;
        maxfd = max(maxfd, listenfd);
        FD_SET(listenfd, &rset);
    }
    
    bind(listenfd, (sockaddr*)&srvaddr, sizeof(srvaddr));
    listen(listenfd, 10);

    while(1) {
        fd_set nrset = rset;
        int nready = select(maxfd + 1, &nrset, NULL, NULL, NULL);
        if(nready == -1 && errno == EINTR)
            continue;
        if(FD_ISSET(listenfd, &nrset)) {
            printf("listenfd is ready\n");
            int connfd = accept(listenfd, NULL, NULL);
            int index = search(-1, connetfd, FD_SETSIZE);
            if(index < FD_SETSIZE) {
                connetfd[index] = connfd;
                maxfd = max(maxfd, connfd);
                FD_SET(connfd, &rset);
            }
            --nready;
        }
        char buff[1024];
        for(int i = 0; i < maxfd + 1 && nready; i++)
            if(FD_ISSET(i, &nrset)) {
                printf("num %d fd is ready\n", i);
                int n = read(i, buff, 1024);
                if(n == -1 && errno == EINTR) continue;
                if(n == 0) {
                    close(i);
                    int index = search(i, connetfd, FD_SETSIZE);
                    connetfd[index] = -1;
                    FD_CLR(i, &rset);
                }
                else
                    write(i, buff, n);
                nready--;
            }
    }
    return 0;
}
