#include "server.h"
#include "epoll_run.h"
#include "socket.h"
#include "channel.h"
#include "endpoint.h"
#include "acceptor.h"
#include "connection.h"
#include "threadPool.h"

Server::Server(Socket* server, EpollRun* _er): er(_er), acceptor(nullptr) {
    // one loop in a thread
    this->acceptor = new Acceptor(server, er);
    acceptor->setNewConnectionHandle(std::bind(&Server::newConnectionHandle, this, std::placeholders::_1));
    tp = new ThreadPool();
    int size = std::thread::hardware_concurrency();
    for(int i = 0; i < size; i++) {
        // 创建sub—reactor等待处理连接
        EpollRun* sub_ep = new EpollRun();
        sub_reactors.emplace_back(sub_ep);
    }

    for(int i = 0; i < size; i++) {
        std::function<void()>sub_loop = std::bind(&EpollRun::run, sub_reactors[i]);
        tp->add(sub_loop);
    }
}

Server::~Server() {}

void Server::newConnectionHandle(Socket* client) {
    // load balance schedule algothrm
    // ramdom schedule
    
    int random = client->get_fd() % sub_reactors.size();
    // create a new connection
    Connection* newConn = new Connection(client, sub_reactors[random]);
    newConn->setDisconnectClient(std::bind(&Server::disconnectHandle, this, std::placeholders::_1));
    // add connection to connections
    connections[newConn->get_id()] = std::move(newConn);
}

void Server::disconnectHandle(int conn_id) {
    // remove connection from connections
    // std::printf("disconnect %d connection\n", conn_id);
    if (connections.find(conn_id) != connections.end()) {
        Connection* conn = connections[conn_id];
        connections.erase(conn_id);
        close(conn->get_fd());
        //delete conn;  //epoll 仍然可poll 导致段错误
    }
    // printf("disconnect connection finish\n");
    //delete connection
}
