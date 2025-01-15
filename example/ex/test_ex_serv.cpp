#include "util.h"
#include "merror.h"
#include "endpoint.h"
#include "socket.h"
#include "epoll_run.h"
#include "server.h"
#include "common.h"
#include "connection.h"

static const char SERVER_IP[] = "0.0.0.0";
static const uint16_t SERVER_PORT = 6667;
static const int BACKLOG = 10;

class ExchangeServer {
public:
    ExchangeServer(EventLoop* main_reactor, const char* IP, const uint16_t PORT, const int BACKLOG);
    ~ExchangeServer();

    void start();
private:
    EventLoop* main_reactor_;
    std::unique_ptr<Server> server_;
};

ExchangeServer::ExchangeServer(EventLoop* main_reactor, const char* IP, const uint16_t PORT, const int BACKLOG) {
    main_reactor_ = main_reactor;
    server_ = std::make_unique<Server>(main_reactor_, IP, PORT, BACKLOG);
}

ExchangeServer::~ExchangeServer() {}

void ExchangeServer::start() {
    server_->start();
}

int main(void)
{

    EventLoop* main_reactor = new EventLoop();
    Server* server = new Server(main_reactor, SERVER_IP, SERVER_PORT, BACKLOG);
    
    server->start();
    return 0;
}