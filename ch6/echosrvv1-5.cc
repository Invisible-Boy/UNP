#include <sys/socket.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdio>
#include <memory.h>
#include <unistd.h>
#include <errno.h>

int main() {
    sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(29732);
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    const int OPEN_MAX = sysconf(_SC_OPEN_MAX);
    pollfd rset[OPEN_MAX];
    for(int i = 0; i < OPEN_MAX; i++)
        rset[i].fd = -1;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(listenfd, (sockaddr*)&srvaddr, sizeof(srvaddr));
    listen(listenfd, 10);

    rset[0].fd = listenfd;
    rset[0].events = POLLRDNORM;
    
    char buff[1024];
    printf("%d\n", OPEN_MAX);
    while(1) {
        int nready = poll(rset, 257, 100000);
        if(nready < 0) {
            printf("errno = %s\n", strerror(errno));
            return 0;
        }
        if(rset[0].revents & POLLRDNORM) {
            nready--;
            int connectfd = accept(listenfd, NULL, NULL);
            for(int i = 1; i < OPEN_MAX; i++)
                if(rset[i].fd == -1) {
                    rset[i].fd = connectfd;
                    rset[i].events = POLLRDNORM;
                    break;
                }
        }
        for(int i = 1; i < OPEN_MAX && nready; i++) {
            int &nowfd = rset[i].fd;
            if(nowfd > -1 && (rset[i].revents & (POLLRDNORM | POLLERR))) {
                nready--;
                int n = read(nowfd, buff, sizeof(buff));
                if(n < 0) {
                    if(errno == ECONNRESET) {
                        close(nowfd);
                        nowfd = -1; 
                    }
                    else
                        printf("read error!\n");
                }
                else if(n == 0) {
                    close(nowfd);
                    nowfd = -1;
                }
                else
                    write(nowfd, buff, n);
            }
        }
    }
    return 0;
}
