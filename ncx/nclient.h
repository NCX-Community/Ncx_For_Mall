#ifndef NCLIENT_H
#define NCLIENT_H
#include "util.h"

extern char* NSERV_IP = "127.0.0.1";
extern uint16_t NSERV_PORT = 7000;
extern char* CLI_IP = "127.0.0.1";
extern uint16_t CLI_PORT = 0;
extern char* LOCAL_SERV_IP = "127.0.0.1";
extern uint16_t LOCAL_SERV_PORT = 7001;

class NClient {
public: 
    NClient() = default;
    void run_client();
private:
    std::unique_ptr<ControlChannelsMap> ccmap_;
};

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
    std::mutex mtx;
    std::unordered_map<int, std::unique_ptr<ControlChannelHandle>> ccmap;
};

class ControlChannelHandle
{
public: 
    ControlChannelHandle();
};

class ControlChannel
{
public:
    ControlChannel();
    ~ControlChannel();
    void run();
private:
    std::unique_ptr<EventLoop> loop_;
    std::shared_ptr<Connection> conn_; 
};

#endif