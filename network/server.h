/*
NCX Server: NCX的服务端代码
*/
#ifndef SERVER_H
#define SERVER_H

#include"util.h"

// Reactor模式中的reactor，负责监听事件和分发事件
class Server {
public:
    Server() = default;
    Server(EventLoop* pool, const char* IP, const uint16_t PORT, const int BACKLOG);
    ~Server();

    void start();

    //监听体事件函数集
    void newConnectionHandle(int client_fd);
    void disconnectHandle(const std::shared_ptr<Connection>& conn);
    void disconnectHandleInLoop(const std::shared_ptr<Connection>& conn);

    //连接体回调函数
    void bind_on_connect(std::function<void(std::shared_ptr<Connection>)> func);
    void bind_on_message(std::function<void(std::shared_ptr<Connection>)> func);

private:
    EventLoop* loop_;
    std::unique_ptr<Acceptor> server_acceptor_; //监听客户端连接

    std::unordered_map<int, std::shared_ptr<Connection>> connections; //连接池

    // 连接握手阶段，用于对连接的预处理（鉴权、判断连接请求类型）
    std::function<void(std::shared_ptr<Connection>)> on_connect_;
    
    std::function<void(std::shared_ptr<Connection>)> on_message_;

};

#endif