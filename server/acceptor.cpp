#include"acceptor.h"
#include"epoll_run.h"
#include"socket.h"
#include"channel.h"
#include"endpoint.h"
#include"server.h"

Acceptor::Acceptor(Socket* _sock, EpollRun* _er) {
    this->er = _er;
    this->sock = _sock;
    int serv_fd = sock->get_fd();
    this->acceptChanel = new Channel(er, serv_fd); 
    acceptChanel->setReadHandleFunc(std::bind(&Acceptor::acceptNewConnection, this));
    acceptChanel->enableRead();
    acceptChanel->setET();
}

void Acceptor::acceptNewConnection() {
    printf("accept new connection\n");
    Endpoint client_addr;
    Socket* client_sock = new TcpSocket(sock->accept(client_addr));
    newConnectionHandle(client_sock);
}

void Acceptor::setNewConnectionHandle(std::function<void(Socket*)> handle) {
    newConnectionHandle = handle;
}