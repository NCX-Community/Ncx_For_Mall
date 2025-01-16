#include"connector.h"
#include"socket.h"
#include"socketops.h"
#include"EventLoop.h"
#include"channel.h"
#include<errno.h>

Connector::Connector(EventLoop* loop, const InetAddress& server_addr)
    : loop_(loop),
      server_addr_(server_addr),
      connect_(false),
      state_(kDisconnected),
      socket_(new TcpSocket(true))
{}

Connector::~Connector() {}

void Connector::start() {
    connect_ = true;
    loop_->run_on_onwer_thread(std::bind(&Connector::startInLoop, this));
}

void Connector::stop()
{
    connect_ = false;
    loop_->run_on_onwer_thread(std::bind(&Connector::stopInLoop, this));
}

void Connector::stopInLoop()
{
    if (state_ == kConnecting)
    {
        setState(kDisconnected);
        int sockfd = removeAndResetChannel();
        Close(sockfd);
    }
}

void Connector::startInLoop() {
    assert(state_ == kDisconnected);
    if (connect_) {
        int sockfd = connect();
        setState(kConnected);
        new_conn_cb_(sockfd); // 连接成功，回调返回连接成功的fd
    }
    else {
        std::puts("Connector::startInLoop() is called when connect_ is false");
    }
}

// 连接函数
int Connector::connect() {
    socket_->connect(server_addr_);
    return socket_->get_fd();
    // TODO: 实现断线重连尝试
}

int Connector::removeAndResetChannel() {
    channel_->disableAll();
    channel_->remove();
    int sockfd = channel_->getFd();
    loop_->run_on_onwer_thread(std::bind(&Connector::resetChannel, this));

    return sockfd;
}

void Connector::resetChannel() {
    channel_.reset();
}