// socket.h
#ifndef SOCKET_H
#define SOCKET_H
#include "net_util/endpoint.h"

// socket trait
class Socket
{
public:
    virtual ~Socket() = default;
    virtual void bind(Endpoint local_endpoint);
    virtual void connect(Endpoint remote_endpoint);
    virtual void listen(int backlog);
    virtual int accept(Endpoint &client_endpoint);
    virtual void close();
    virtual Endpoint get_endpoint();
    virtual Endpoint get_remote_endpoint();
};

// tcp Socket
class TcpSocket : public Socket
{
private:
    int fd;
    int cli_fd;
public:
    TcpSocket();
    ~TcpSocket();
    void bind(Endpoint local_endpoint) override;
    void connect(Endpoint remote_endpoint) override;
    void listen(int backlog) override;
    int accept(Endpoint &client_endpoint) override;
    void close() override;
    Endpoint get_endpoint() override;
    Endpoint get_remote_endpoint() override;
};

#endif