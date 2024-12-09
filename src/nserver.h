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

private:
    EpollRun *main_reactor_;
    std::unique_ptr<Server> main_acceptor_;
};

/// control channel handle
class ControlChannelHandle
{
public:
    ControlChannelHandle(std::shared_ptr<Connection> conn);
    void run_tcp_pool(const char *IP, const uint16_t PORT, std::shared_ptr<MuslChannelTx> data_ch_req_tx,
                        MuslChannelRx* data_ch_req_rx);

private:
    std::shared_ptr<MuslChannelRx> data_ch_rx;
    std::unique_ptr<EpollRun> tcp_conn_pool_reactor_;
    std::unique_ptr<Server> tcp_conn_pool_;
};

#endif