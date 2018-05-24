#include <sys/signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <unistd.h>
#include <memory.h>
#include <sys/errno.h>
#include <cstdio>

using namespace std;

const int Nmax = 1024;

void srvfunc(int connfd) {
    int n = 0;
    char buff[Nmax];
again:
    while((n = read(connfd, buff, sizeof(buff))) > 0)
        write(connfd, buff, n);
    if(n < 0 && errno == EINTR)
        goto again;
    else if(n < 0)
        printf("read error!\n");
}

void handler(int param) {
    int pid;
    while((pid = waitpid(-1, NULL, WNOHANG)) > 0)
        printf("child %d is killed.\n", pid);
}

int main() {
    sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(29732);
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int listenfd = socket(AF_INET, SOCK_STREAM, NULL);
    bind(listenfd, (sockaddr*)&srvaddr, sizeof(srvaddr));
    listen(listenfd, 10);

    signal(SIGCHLD, handler);

    while(1) {
        int connfd = accept(listenfd, NULL, NULL);
        int pid;
        if((pid = fork()) == 0) {
            close(listenfd);
            srvfunc(connfd);
            return 0;
        }
        close(connfd);
    }
    return 0;
}
