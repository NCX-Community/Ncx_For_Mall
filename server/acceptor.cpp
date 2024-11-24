#include"acceptor.h"
#include"epoll_run.h"
#include"socket.h"
#include"channel.h"
#include"endpoint.h"
#include"server.h"

Acceptor::Acceptor(const char* IP, const uint16_t PORT, const int BACKLOG, EpollRun* _er) {
    // create tcp server
    Endpoint server_endpoint(IP, PORT);
    this->sock = std::make_unique<TcpSocket>();
    sock->bind(server_endpoint);
    sock->listen(BACKLOG);

    this->er = _er;
    this->acceptChanel = new Channel(er, sock->get_fd()); 
    acceptChanel->set_read_callback(std::bind(&Acceptor::acceptNewConnection, this));
    acceptChanel->enableRead();
    acceptChanel->set_et();
}

void Acceptor::acceptNewConnection() {
    printf("accept new connection\n");
    Endpoint client_addr;
    if(new_connection_callback_) {
        new_connection_callback_(sock->accept(client_addr));
    }
    else std::puts("new connection callback not init");
}

void Acceptor::set_new_connection_callback(std::function<void(int)> handle) {
    new_connection_callback_ = handle;
}