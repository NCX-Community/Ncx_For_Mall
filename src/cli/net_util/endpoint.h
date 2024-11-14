#ifndef ENDPOINT_H
#define ENDPOINT_H

#include<arpa/inet.h>
#include <string.h>
#include <stdio.h>

class Endpoint{
public:
    sockaddr_in addr;
    Endpoint(const char* ip, uint16_t port);
};

#endif