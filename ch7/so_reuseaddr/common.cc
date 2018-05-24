#include "common.h"

void print(sockaddr_in *addr) {
    char ip[256];
    inet_ntop(addr->sin_family, (void*)&addr->sin_addr.s_addr, ip, 255);
    printf("%s : %d\n", ip, ntohs(addr->sin_port));
}
