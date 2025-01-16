#ifndef ENDPOINT_H
#define ENDPOINT_H

#include"util.h"

class InetAddress{
public:
    InetAddress(const char* ip, uint16_t port);
    InetAddress(sockaddr_in ip_addr);
    struct sockaddr_in addr;
};

#endif