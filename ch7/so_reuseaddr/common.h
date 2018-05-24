#ifndef COMMON_H
#define COMMON_H

#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <memory.h>
#include <unistd.h>
#include <cstdio>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <cstring>
#include <sys/select.h>
#include <sys/types.h>

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

void print(sockaddr_in*);

#endif
