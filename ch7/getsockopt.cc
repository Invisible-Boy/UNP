#include <iostream>
#include <sys/socket.h>

using namespace std;

int main() {
    int tcpfd = socket(AF_INET, SOCK_STREAM, 0);
    int udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    int bufsize = 0;
    socklen_t len = sizeof(bufsize);
    getsockopt(tcpfd, SOL_SOCKET, SO_SNDBUF, (void*)&bufsize, &len);
    cout << "tcp send buff size is " << bufsize << endl;
    getsockopt(udpfd, SOL_SOCKET, SO_SNDBUF, (void*)&bufsize, &len);
    cout << "udp send buff size is " << bufsize << endl;
    getsockopt(tcpfd, SOL_SOCKET, SO_RCVBUF, (void*)&bufsize, &len);
    cout << "tcp rcv buff size is " << bufsize << endl;
    getsockopt(udpfd, SOL_SOCKET, SO_RCVBUF, (void*)&bufsize, &len);
    cout << "udp rcv buff size is " << bufsize << endl;
    return 0;
}
