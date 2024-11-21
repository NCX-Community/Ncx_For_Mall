// socket.h
#ifndef SOCKET_H
#define SOCKET_H
#include"util.h"

// socket trait
class Socket
{
public:
    Socket() = default;
    virtual ~Socket() = 0;
    virtual void bind(Endpoint local_endpoint) = 0;
    virtual void connect(Endpoint remote_endpoint) = 0;
    virtual void listen(int backlog) = 0;
    virtual int accept(Endpoint &client_endpoint) = 0;
    // virtual void close();
    // virtual Endpoint get_endpoint();
    // virtual Endpoint get_remote_endpoint();
    virtual int get_fd() = 0;
};

// tcp Socket
class TcpSocket : public Socket
{
private:
    int fd;
    int cli_fd;
public:
    TcpSocket();
    ~TcpSocket() override;
    void bind(Endpoint local_endpoint) override;
    void connect(Endpoint remote_endpoint) override;
    void listen(int backlog) override;
    int accept(Endpoint &client_endpoint) override;
    // void close() override;
    // Endpoint get_endpoint() override;
    // Endpoint get_remote_endpoint() override;
    int get_fd() override;
};


Socket* create_socket(Type sock_tp);
#endif