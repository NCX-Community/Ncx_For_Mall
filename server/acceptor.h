#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include"util.h"

class Acceptor {
public: 
    Acceptor() = default;
    Acceptor(Socket* _sock, EpollRun* _er);
    void acceptNewConnection();
    void setNewConnectionHandle(std::function<void(Socket*)> handle);
private:
    EpollRun* er;
    Socket* sock;
    Channel* acceptChanel;
    std::function<void(Socket*)> newConnectionHandle;
};

#endif