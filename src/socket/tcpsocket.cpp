#include<sys/socket.h>
#include "socket.h"
#include "error/error.h"

TcpSocket::TcpSocket() {
    fd = socket(AF_INET, SOCK_STREAM, 0);
}

TcpSocket::~TcpSocket() {
    close();
}

void TcpSocket::bind(Endpoint local) {
    errif(::bind(fd, (sockaddr*)&local.addr, sizeof(local.addr)), "tcp bind error");
}

void TcpSocket::connect(Endpoint remote) {
    errif(::connect(fd, (sockaddr*)&remote.addr, sizeof(remote.addr)), "tcp connect error");
}

void TcpSocket::listen(int backlog) {
    errif(::listen(fd, backlog), "tcp listen error");
}

int TcpSocket::accept(Endpoint &client) {
    struct sockaddr_in client_addr;
    socklen_t client_ip_len = sizeof(client_addr);
    bzero(&client_addr, client_ip_len);
    int client_fd = ::accept(fd, (sockaddr*)&client_addr, &client_ip_len);
}
