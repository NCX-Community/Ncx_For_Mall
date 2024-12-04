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

    void bind_on_connect(std::function<int(int)> func);
    void bind_on_message(std::function<void(std::shared_ptr<Connection>)> func);
    void bind_on_conn_read(std::function<void(std::shared_ptr<Connection>)> func);
    void bind_on_conn_write(std::function<void(std::shared_ptr<Connection>)> func);

    //set connection exchange pair
    void exchange_pair(int conn_id1, int conn_id2);

private:
    std::unique_ptr<EpollRun> main_reactor_;
    std::unique_ptr<Acceptor> acceptor;
    std::unique_ptr<EpThreadPool> tp;     //线程池应该由reactor负责管理
    std::unordered_map<int, std::shared_ptr<Connection>> connections;

    std::function<int(int)> on_connect_;
    std::function<void(std::shared_ptr<Connection>)> on_message_;

    // 对外暴露conntion读写事件处理接口
    std::function<void(std::shared_ptr<Connection>)> on_conn_read_;
    std::function<void(std::shared_ptr<Connection>)> on_conn_write_;
};

#endif