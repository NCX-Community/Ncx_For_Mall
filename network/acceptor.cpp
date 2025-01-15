#include"acceptor.h"
#include"EventLoop.h"
#include"socket.h"
#include"channel.h"
#include"InetAddress.h"
#include"server.h"

Acceptor::Acceptor(const char* IP, const uint16_t PORT, const int BACKLOG, EventLoop* _er) {
    // create tcp server
    InetAddress server_endpoint(IP, PORT);
    this->sock = std::make_unique<TcpSocket>(false);
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
    InetAddress client_addr;
    if(new_connection_callback_) {
        new_connection_callback_(sock->accept(client_addr));
    }
    else std::puts("new connection callback not init");
}

// server.cpp set
void Acceptor::set_new_connection_callback(std::function<void(int)> handle) {
    new_connection_callback_ = handle;
}