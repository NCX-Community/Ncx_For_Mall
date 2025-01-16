#ifndef SOCKETOPS_H
#define SOCKETOPS_H

#include <arpa/inet.h>

void Close(int sockfd);
struct sockaddr_in getLocalAddr(int sockfd);
struct sockaddr_in getPeerAddr(int sockfd);

#endif