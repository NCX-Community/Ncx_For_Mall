#ifndef CCONTROLCHANNEL_H
#define CCONTROLCHANNEL_H

#include "util.h"
#include "InetAddress.h"
#include "client.h"

struct CControlChannelArgs {
    InetAddress server_addr_;
    InetAddress service_addr_;
};

class CControlChannel 
{
public:
    CControlChannel() = delete;
    CControlChannel(EventLoop* loop, std::string id, CControlChannelArgs args);
    ~CControlChannel() = default;

    // 控制通道回调函数集合
    // 握手阶段(建立连接后触发)
    void do_control_channel_handshake(std::shared_ptr<Connection> server_conn);
    void wait_control_channel_hello(std::shared_ptr<Connection> server_conn, Buffer* conn_input_buf);
    void handle_control_channel_hello(std::string nonce);
    void wait_ack(std::shared_ptr<Connection> server_conn, Buffer* conn_input_buf);
    void handle_ack(std::shared_ptr<Connection> server_conn);
    std::function<void(std::string, std::string)> register_control_channel_;

    // 向服务端发起连接建立控制通道
    void start();
private:
    std::string id_;
    EventLoop* loop_;
    CControlChannelArgs args_;
    // 负责和NCX Server通信、发送处理control cmd
    std::unique_ptr<Client> control_client_;
};

#endif