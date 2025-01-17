#ifndef NSERVER_H
#define NSERVER_H

#include "util.h"
#include "scontrolchannel.h"
#include "server.h"
#include "connection.h"
#include "EventLoop.h"
#include "protocol.pb.h"

typedef std::unordered_map<int, std::unique_ptr<SControlChannel>> SControlChannelMap;

struct ServerArgs{
    InetAddress server_addr_;
    int backlog_ {256};
};

class NServer {
public:
    DISALLOW_COPY_AND_MOVE(NServer);
    NServer(ServerArgs args);

    /// control channel 建立连接回调函数集
    void handle_new_connection(std::shared_ptr<Connection> conn, Buffer* conn_input_buf);
    void handle_control_channel_handshake(std::shared_ptr<Connection> conn);
    void handle_data_channel_handshake(std::shared_ptr<Connection> conn);

    void run_server();

private:
    std::unique_ptr<EventLoop> loop_;
    ServerArgs args_;   // 服务器参数集合

    /// NCX server 负责监听并处理来自客户端的control client, 完成control channel的建立
    std::shared_ptr<Server> ncx_acceptor_;

    /// 控制通道集合
    std::unique_ptr<SControlChannelMap> sc_map_;
};

#endif