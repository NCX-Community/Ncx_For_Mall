#include "nclient.h"
#include "endpoint.h"
#include "socket.h"
#include "protocol.h"
#include "connection.h"

NClient::NClient() {
    // bind and connect to server
    Endpoint remote_endpoint(NSERV_IP, NSERV_PORT);
}

void NClient::run_client() {
}

/// Control Channels Map Guard
void ControlChannelsMap::add_control_channel(int id, ControlChannelHandle* cc) {
    std::lock_guard<std::mutex> lock(mtx);
    ccmap[id] = std::unique_ptr<ControlChannelHandle>(cc);
}

void ControlChannelsMap::remove_control_channel(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    ccmap.erase(id);
}

ControlChannelHandle* ControlChannelsMap::get_control_channel(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    return ccmap[id].get();
}

/// Control Handle
ControlChannelHandle::ControlChannelHandle() {
    // create control channel
    std::unique_ptr<ControlChannel> control_channel = std::make_unique<ControlChannel>();
    std::function<void()> thread_func = [control_channel = std::move(control_channel)]() {
        control_channel->run();
    };

    std::thread control_pool = std::thread(thread_func);
}


/// Control Channel
ControlChannel::ControlChannel(): conn_(nullptr) {
    loop_ = std::make_unique<EpollRun>();
}

void ControlChannel::run() {
    // connect remote
    Endpoint remote = Endpoint(NSERV_IP, NSERV_PORT);
    Endpoint local = Endpoint(CLI_IP, CLI_PORT);
    std::unique_ptr<TcpSocket> client = std::make_unique<TcpSocket>(true);
    client->connect(remote);

    //init connection
    conn_ = std::make_unique<Connection>(client->get_fd(), loop_.get());
    conn_->set_data_in_handle([](const std::shared_ptr<Connection
    > conn){
        // init handle read event: read hello
        PROTOCOL::Hello hello;
        conn->Recv((char*)&hello, sizeof(hello));
    });
    conn_->ConnectionEstablished();

    // send hello
    PROTOCOL::Hello hello = PROTOCOL::Hello::ControlChannelHello;
    conn_->Send((char*)&hello, sizeof(hello));

    // renew read event handle: read control channel command and handle it
    conn_->set_data_in_handle([](const std::shared_ptr<Connection> conn){
        // 服务端有命令传来
        PROTOCOL::ControlChannelCmd cmd;
        conn->Recv((char*)&cmd, sizeof(cmd));

        if(cmd == PROTOCOL::ControlChannelCmd::CreateDataChannel) {
            
        }
        else if(cmd == PROTOCOL::ControlChannelCmd::HeartBeat) {
            {}
        }

    });
}

std::shared_ptr<Connection> do_data_channel_shake(EpollRun* loop) {
    // create new connect remote
    Endpoint remote = Endpoint(NSERV_IP, NSERV_PORT);
    Endpoint local = Endpoint(CLI_IP, CLI_PORT);
    std::unique_ptr<TcpSocket> client = std::make_unique<TcpSocket>(true);
    client->connect(remote);
    printf("data channel connected\n");

    //init connection
    std::shared_ptr<Connection> data_conn = std::make_unique<Connection>(client->get_fd(), loop);
    data_conn->set_data_in_handle([loop](const std::shared_ptr<Connection
    > conn){
        // conn is data_conn
        // init handle read event: read data transfer 
        PROTOCOL::DataChannelCmd cmd;
        conn->Recv((char*)&cmd, sizeof(cmd));
        
        if(cmd == PROTOCOL::DataChannelCmd::StartForwardTcp) {
            // start forward tcp
            Endpoint local = Endpoint(LOCAL_SERV_IP, LOCAL_SERV_PORT);
            Endpoint client = Endpoint(CLI_IP, CLI_PORT);
            TcpSocket* local_sock = new TcpSocket(true);
            local_sock->bind(client);
            local_sock->connect(local);
            // 这里应该有线程池来处理所有的转发、需要修改优化

            // init connection
            std::shared_ptr<Connection> local_conn = std::make_shared<Connection>(local_sock->get_fd(), loop);
            // set data in exchange between local_conn and conn

        }
        else if(cmd == PROTOCOL::DataChannelCmd::StartForwardUdp) {
            // start forward udp
            // todo 
        }
    });
    data_conn->ConnectionEstablished();

    // send hello
    PROTOCOL::Hello hello = PROTOCOL::Hello::DataChannelHello;
    data_conn->Send((char*)&hello, sizeof(hello));

    return data_conn->shared_from_this();

}

void run_data_channel(std::shared_ptr<Connection> control_conn) {
    std::shared_ptr<Connection> data_conn = do_data_channel_shake(control_conn->get_epoll_run());
}

void run_data_channel_for_tcp(Connection* conn1, Connection* conn2) {
    // conn1 and conn2 双向转发
}