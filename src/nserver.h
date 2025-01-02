#ifndef NSERVER_H
#define NSERVER_H

#include "util.h"
const int TCP_POOL_SIZE = 8;
char *BIND_IP = "127.0.0.1";
uint16_t SERV_PORT = 7000;
uint16_t BIND_PORT = 8000;

class NCXServer
{
public:
    NCXServer(EpollRun *main_reator, char *IP, uint16_t PORT, int BACKLOG);
    ~NCXServer() = default;
    DISALLOW_COPY_AND_MOVE(NCXServer);

    void run_server();

    // handshake
    void do_channel_handleshake(Connection* conn, ControlChannelsMap* ccmap);
    void do_data_channel_handleshake(Connection* conn, ControlChannelsMap* ccmap);

private:
    EpollRun *main_reactor_;
    std::unique_ptr<Server> main_acceptor_;
    std::unique_ptr<ControlChannelsMap> ccmap_;
};

/// control channels map
class ControlChannelsMap
{
public:
    ControlChannelsMap() = default;
    ~ControlChannelsMap() = default;
    DISALLOW_COPY_AND_MOVE(ControlChannelsMap);

    void add_control_channel(int id, ControlChannelHandle* cc);
    void remove_control_channel(int id);
    ControlChannelHandle* get_control_channel(int id);

private:
    // id目前先设置为visitor连接体的id
    std::unordered_map<int, std::unique_ptr<ControlChannelHandle>> ccmap;
    std::mutex mtx;
};

/// control channel handle
class ControlChannelHandle
{
public:
    std::shared_ptr<MuslChannelTx> data_ch_tx_;
    ControlChannelHandle(std::shared_ptr<Connection> conn);
    void run_tcp_pool(const char *IP, const uint16_t PORT, std::shared_ptr<MuslChannelTx> data_ch_req_tx,
                        std::unique_ptr<MuslChannelRx> data_ch_rx);

private:
    std::unique_ptr<EpollRun> tcp_conn_pool_reactor_;
    std::unique_ptr<Server> tcp_conn_pool_;
};


// controlChannel
class ControlChannel {
public:
    ControlChannel(Connection* conn, MuslChannelRx* data_ch_rx);
    void run();
private:
    std::shared_ptr<Connection> conn_;
    std::unique_ptr<MuslChannelRx> data_ch_rx_;
};

#endif