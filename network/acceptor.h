#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include"util.h"

class Acceptor {
public: 
    Acceptor() = default;
    Acceptor(const char* IP, const uint16_t PORT, const int BACKLOG, EventLoop* _er);
    Acceptor(const InetAddress& addr, const int BACKLOG, EventLoop* _er);
    void acceptNewConnection();
    void set_new_connection_callback(std::function<void(int)> handle);
private:
    EventLoop* er;
    std::unique_ptr<TcpSocket> sock;
    Channel* acceptChanel;
    std::function<void(int)> new_connection_callback_;
};

#endif