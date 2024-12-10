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
    bool is_nonblock_;
public:
    TcpSocket(bool is_nonblock = true);
    TcpSocket(int _fd);
    ~TcpSocket();
    void bind(Endpoint local_endpoint);
    void connect(Endpoint remote_endpoint);
    bool connect_on_nonblocking(Endpoint remote_endpoint);
    void listen(int backlog);
    int accept(Endpoint &client_endpoint);
    // void close() override;
    // Endpoint get_endpoint() override;
    // Endpoint get_remote_endpoint() override;
    int get_fd();
};
#endif