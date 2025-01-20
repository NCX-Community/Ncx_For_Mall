#include "InetAddress.h"

InetAddress::InetAddress(const char *ip, uint16_t port)
{
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
}

InetAddress::InetAddress(sockaddr_in ip_addr): addr(ip_addr) {}

void InetAddress::print() const { printf("ip: %s, port: %d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port)); }