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
    bool is_nonblock_;
    bool connect_on_nonblocking(Endpoint remote_endpoint);
public:
    TcpSocket(bool is_nonblock = true);
    TcpSocket(int _fd);
    ~TcpSocket();
    void bind(Endpoint local_endpoint);
    void connect(Endpoint remote_endpoint);
    void listen(int backlog);
    int accept(Endpoint &client_endpoint);
    int get_fd();

    size_t read(char *buf);
    size_t write(const char *buf);
    
};
#endif