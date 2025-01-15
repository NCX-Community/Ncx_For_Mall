#include "InetAddress.h"

InetAddress::InetAddress(const char *ip, uint16_t port)
{
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = port;
    
}

InetAddress::InetAddress(sockaddr_in ip_addr): addr(ip_addr) {}