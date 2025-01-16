#ifndef CONNECTOR_H
#define CONNECTOR_H
#include"util.h"
#include"InetAddress.h"

/// 连接器
/// 负责向服务端发起连接
class Connector {
public:
    Connector(EventLoop* loop, const InetAddress& server_addr);
    ~Connector();

    void setNewConnectionCb(const std::function<void(int sockfd)>& cb) { new_conn_cb_ = cb; }

    void start();
    void restart(); // TODO: 等到定时器实现后再实现
    void stop();    // TODO

    const InetAddress& serverAddr() const { return server_addr_; }

private:
    enum States { kDisconnected, kConnecting, kConnected };

    void setState(States s) { state_ = s; }
    void startInLoop();
    void stopInLoop();
    int connect();
    void connecting(int sockfd);
    int removeAndResetChannel();
    void resetChannel(); 


    EventLoop* loop_;
    InetAddress server_addr_;
    bool connect_;
    std::unique_ptr<TcpSocket> socket_;
    std::unique_ptr<Channel> channel_;
    std::function<void(int sockfd)> new_conn_cb_;
    States state_;

};

#endif
