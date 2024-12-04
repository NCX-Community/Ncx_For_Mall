#include "util.h"
#include "merror.h"
#include "endpoint.h"
#include "socket.h"
#include "epoll_run.h"
#include "connection.h"
#include "server.h"
#include "common.h"

static const char SERVER_IP[] = "0.0.0.0";
static const uint16_t SERVER_PORT = 6667;
static const int BACKLOG = 10;

class EchoServer {
public:
    EchoServer(EpollRun* main_reactor, const char* IP, const uint16_t PORT, const int BACKLOG);
    ~EchoServer();
    void start();
    void on_conn_read(const std::shared_ptr<Connection>& conn);
private:
    EpollRun* main_reactor_;
    std::unique_ptr<Server> server_;
};

EchoServer::EchoServer(EpollRun* main_reactor, const char* IP, const uint16_t PORT, const int BACKLOG) {
    main_reactor_ = main_reactor;
    server_ = std::make_unique<Server>(main_reactor_, IP, PORT, BACKLOG);

    server_->bind_on_conn_read([this](const std::shared_ptr<Connection>& conn) {
        this->on_conn_read(conn);
    });
}

EchoServer::~EchoServer() {}

void EchoServer::on_conn_read(const std::shared_ptr<Connection>& conn) {
    if(conn->get_state() == ConnectionState::CONNECTED) {
        conn->Read();
        if(conn->get_input_buffer()->size() == 0) {
            return;
        }
        printf("read from client: %s\n", conn->get_input_buffer()->data());
        conn->get_output_buffer()->clear();
        conn->get_output_buffer()->append(conn->get_input_buffer()->data(), conn->get_input_buffer()->size());
        conn->Write();
    }
}
void EchoServer::start() {
    server_->start();
}

int main(void)
{
    // echo server
    EpollRun* main_reactor = new EpollRun();
    EchoServer* echoServer = new EchoServer(main_reactor, SERVER_IP, SERVER_PORT, BACKLOG);
    echoServer->start();

    delete main_reactor;
    return 0;
}