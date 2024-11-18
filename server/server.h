#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>

// Reactor模式中的reactor，负责监听事件和分发事件
class EpollRun;
class Socket;

class Server {
private:
    EpollRun* er;
    Socket* server;
public:
    Server() = default;
    Server(Socket* serv_sock, EpollRun* er);
    ~Server();
    //handle 事件函数集
    void handleNewConnection();
    void handleRead(int client_fd);
};

#endif