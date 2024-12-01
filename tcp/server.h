#ifndef SERVER_H
#define SERVER_H

#include"util.h"

// Reactor模式中的reactor，负责监听事件和分发事件

class Server {
public:
    Server() = default;
    Server(EpollRun* pool, const char* IP, const uint16_t PORT, const int BACKLOG);
    ~Server();

    void start();

    //handle 事件函数集
    void newConnectionHandle(int client_fd);
    void disconnectHandle(const std::shared_ptr<Connection>& conn);
    void disconnectHandleInLoop(const std::shared_ptr<Connection>& conn);

    void bind_on_connect(std::function<void(std::shared_ptr<Connection>)> func);
    void bind_on_message(std::function<void(std::shared_ptr<Connection>)> func);
private:
    std::unique_ptr<EpollRun> main_reactor_;
    std::unique_ptr<Acceptor> acceptor;
    std::unique_ptr<EpThreadPool> tp;     //线程池应该由reactor负责管理
    std::unordered_map<int, std::shared_ptr<Connection>> connections;

    std::function<void(std::shared_ptr<Connection>)> on_connect_;
    std::function<void(std::shared_ptr<Connection>)> on_message_;
};

#endif