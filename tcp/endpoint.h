#ifndef ENDPOINT_H
#define ENDPOINT_H

#include"util.h"

class Endpoint{
public:
    sockaddr_in addr;
    Endpoint() = default;
    Endpoint(const char* ip, uint16_t port);
    Endpoint(sockaddr_in ip_addr);
};

#endif