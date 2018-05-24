/**
 *  Author: Yifan Zhang
 *  Email:  zhangyfo@163.com
 *  Remark: Review tcp, udp, select, poll by echo server-client.
 */
#include <sys/signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <memory.h>
#include <cstdio>

int main() {
    sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);


    int srvfd = socket(AF_INET, SOCK_STREAM, 0);
    
    return 0;
}
