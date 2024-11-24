// socket.h
#ifndef SOCKET_H
#define SOCKET_H
#include"util.h"
#include "common.h"
// tcp Socket
class TcpSocket
{
private:
    int fd;
    int cli_fd;
public:
    TcpSocket();
    TcpSocket(int _fd);
    ~TcpSocket();
    void bind(Endpoint local_endpoint);
    void connect(Endpoint remote_endpoint);
    void listen(int backlog);
    int accept(Endpoint &client_endpoint);
    // void close() override;
    // Endpoint get_endpoint() override;
    // Endpoint get_remote_endpoint() override;
    int get_fd();
};
#endif