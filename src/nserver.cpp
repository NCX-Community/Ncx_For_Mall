#include"nserver.h"
#include"musl_channel.h"
#include"server.h"
#include"epoll_run.h"
#include"protocol.h"
#include"connection.h"

/// NSERVER
NCXServer::NCXServer(EpollRun* main_reactor, char* IP, uint16_t PORT, int BACKLOG)
: main_reactor_(main_reactor) {
    main_acceptor_ = std::make_unique<Server>(main_reactor_, IP, PORT, BACKLOG);
    ccmap_ = std::make_unique<ControlChannelsMap>();
    // 初始化on_connect_函数，当有新连接时，需要创建Control Channel handle
    main_acceptor_->bind_on_connect([this, main_acceptor_ = main_acceptor_.get(), ccmap_ = ccmap_.get()](std::shared_ptr<Connection> new_conn) {
        // create control channel handle
        try {
            PROTOCOL::Hello hello = PROTOCOL::read_hello(new_conn->get_fd());
            if(hello == PROTOCOL::Hello::ControlChannelHello) {
                // create control channel handle
                do_channel_handleshake(new_conn.get(), ccmap_);
            }
            else if(hello == PROTOCOL::Hello::DataChannelHello) {
                // create data channel handle
                do_channel_handleshake(new_conn.get(), ccmap_);
            }
            else{
                // error、close connection
                main_acceptor_->disconnectHandle(new_conn);
            }
        }
        catch(const std::exception& e) {
            printf("read hello error: %s\n", e.what());
            main_acceptor_->disconnectHandle(new_conn);
        }
    });
}

void NCXServer::run_server() {
    main_acceptor_->start();
    main_reactor_->run();
}

void NCXServer::do_channel_handleshake(Connection* conn, ControlChannelsMap* ccmap) {
    ControlChannelHandle* cch = new ControlChannelHandle(conn->shared_from_this());
    ccmap->add_control_channel(conn->get_conn_id(), cch);
}

void NCXServer::do_data_channel_handleshake(Connection* conn, ControlChannelsMap* ccmap) {
    int id = conn->get_conn_id();
    ControlChannelHandle* cch = ccmap->get_control_channel(id);
    // 把这个conn发送到数据通道
    cch->data_ch_tx_->send(conn->shared_from_this());
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

/// Control Channel Handle
ControlChannelHandle::ControlChannelHandle(std::shared_ptr<Connection> conn) {
    // create data channel and data req channel
    MuslChannel* data_ch = new MuslChannel();
    data_ch_tx_ = data_ch->getSender();
    std::unique_ptr<MuslChannelRx> data_ch_rx = data_ch->getReceiver();

    MuslChannel* data_req_ch = new MuslChannel();
    std::shared_ptr<MuslChannelTx> data_req_ch_tx = data_ch->getSender();
    std::unique_ptr<MuslChannelRx> data_req_ch_rx = data_ch->getReceiver();

    // cache data channel for later use
    for(int i = 0; i < TCP_POOL_SIZE; i++) {
        data_req_ch_tx->send(true);
    }

    char* bind_ip = BIND_IP;
    uint16_t bind_port = BIND_PORT;
    
    //转发在这里面执行喔
    run_tcp_pool(bind_ip, bind_port, data_req_ch_tx, data_req_ch_rx.get());



}

void ControlChannelHandle::run_tcp_pool(
    const char* IP, 
    const uint16_t PORT,
    std::shared_ptr<MuslChannelTx> data_ch_req_tx,
    MuslChannelRx* data_ch_rx) {
    tcp_conn_pool_reactor_ = std::make_unique<EpollRun>();
    tcp_conn_pool_ = std::make_unique<Server>(tcp_conn_pool_reactor_.get(), IP, PORT, 10);

    // set on visit Connection
    tcp_conn_pool_->bind_on_connect([this, data_ch_req_tx, data_ch_rx](std::shared_ptr<Connection> new_conn) {
        auto cmd = PROTOCOL::DataChannelCmd::StartForwardTcp;
        // 查看是否有空闲的data channel
        auto ch = data_ch_rx->receive<std::shared_ptr<Connection>>();
        if(ch == std::nullopt) {
            // 没有空闲的data channel

        } else {
            // 有空闲的data Channel
            // 向客户端发送数据转发命令
            std::shared_ptr<Connection> ch_conn = ch.value();
            // 这里需要优化、客户端连接如果断连，需要重新申请一个新的data channel
            ch_conn->set_output_buffer(reinterpret_cast<const char*>(&cmd), sizeof(cmd));
            ch_conn->Write();
            ch_conn->flash();
            
            //todo：进行转发
        }
    });

    tcp_conn_pool_->start();
    tcp_conn_pool_reactor_->run();
}


// Control Channel
ControlChannel::ControlChannel(Connection* conn, MuslChannelRx* data_ch_rx) {
    conn_ = std::shared_ptr<Connection>(conn);
    data_ch_rx_ = std::unique_ptr<MuslChannelRx>(data_ch_rx);
}