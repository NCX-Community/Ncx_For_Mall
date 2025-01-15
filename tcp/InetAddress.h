#ifndef ENDPOINT_H
#define ENDPOINT_H

#include"util.h"

class InetAddress{
public:
    sockaddr_in addr;
    InetAddress() = default;
    InetAddress(const char* ip, uint16_t port);
    InetAddress(sockaddr_in ip_addr);
};

#endif