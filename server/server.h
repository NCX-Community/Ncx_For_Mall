#ifndef SERVER_H
#define SERVER_H

#include"util.h"

// Reactor模式中的reactor，负责监听事件和分发事件

class Server {
private:
    EpollRun* er;
    Acceptor* acceptor;
    ThreadPool* tp;     //线程池应该由reactor负责管理
    std::unordered_map<int, Connection*> connections;
    std::vector<EpollRun*> sub_reactors;
public:
    Server() = default;
    Server(Socket* serv_sock, EpollRun* er);
    ~Server();
    //handle 事件函数集
    void newConnectionHandle(Socket* client);
    void disconnectHandle(int conn_id);
    void handleRead(int client_fd);
};

#endif