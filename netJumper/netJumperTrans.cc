#include <sys/socket.h>
#include <string>
#include <map>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdio>
#include <memory.h>
#include <errno.h>
#include <sys/socket.h>
#include <vector>

using namespace std;

void print(sockaddr_in &sendaddr) {
    char ip[1024];
    inet_ntop(AF_INET, &sendaddr.sin_addr.s_addr, ip, 1024);
    printf("%s %d\n", ip, ntohs(sendaddr.sin_port));
}

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(29732);
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sockfd, (sockaddr*)&srvaddr, sizeof(srvaddr));
    map<string, string> nameToMsg;
    map<string, sockaddr_in> nameToSockaddr;
    vector<string> vname;
    while(1) {
        char buff[2048];
        sockaddr_in recvaddr;
        socklen_t len = sizeof(recvaddr);
        int n = recvfrom(sockfd, buff, 2047, 0, (sockaddr*)&recvaddr, &len);
        buff[n] = '\0';
        printf("%s\n", buff);
        printf("recv ip ");
        print(recvaddr);
        char op[2048], nm[2048];
        sscanf(buff, "%s%s", op, nm);
        string name(nm);
        if(nameToSockaddr.count(name) == 0) {
            nameToSockaddr[name] = recvaddr;
            char ip[1024];
            inet_ntop(AF_INET, (void*)&recvaddr.sin_addr.s_addr, ip, 1024);
            sprintf(buff, "%s %s %d", nm, ip, ntohs(recvaddr.sin_port));
            nameToMsg[name] = string(buff);
            vname.push_back(name);
        }
        if(vname.size() == 2) {
            for(int i = 0; i < 2; i++) {
                string msg = nameToMsg[vname[i]];
                sockaddr_in sendaddr = nameToSockaddr[vname[1 - i]];
                sendto(sockfd, msg.c_str(), msg.length(), 0, (sockaddr*)&sendaddr, sizeof(sockaddr_in));
            }
        }
    }
    return 0;
}
