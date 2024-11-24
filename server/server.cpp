#include "server.h"
#include "epoll_run.h"
#include "socket.h"
#include "channel.h"
#include "endpoint.h"
#include "acceptor.h"
#include "connection.h"
#include "threadPool.h"

Server::Server(const char* IP, const uint16_t PORT, const int BACKLOG){
    // create main reactor
    auto main_reactor_ = std::make_unique<EpollRun>();
    auto tp = std::make_unique<ThreadPool>();

    // one loop in a thread
    this->acceptor = std::make_unique<Acceptor>(IP, PORT, BACKLOG, main_reactor_.get());
    acceptor->set_new_connection_callback(std::bind(&Server::newConnectionHandle, this, std::placeholders::_1));

    // create sub-reactor
    int size = std::thread::hardware_concurrency();
    for(int i = 0; i < size; i++) {
        // 创建sub—reactor等待处理连接
        EpollRun* sub_ep = new EpollRun();
        sub_reactors.emplace_back(sub_ep);
    }
}

Server::~Server() {}

void Server::start() {
    int size = std::thread::hardware_concurrency();
    for(int i = 0; i < size; i++) {
        std::function<void()>sub_loop = std::bind(&EpollRun::run, sub_reactors[i].get());
        tp->add(sub_loop);
    }

    main_reactor_->run();
}

void Server::newConnectionHandle(int client_fd) {
    // load balance schedule algothrm
    // ramdom schedule
    
    int random = client_fd % sub_reactors.size();
    // create a new connection
    std::shared_ptr<Connection> newConn = std::make_shared<Connection>((std::move(client_fd), sub_reactors[random].get()));
    newConn->set_disconnect_client_handle(std::bind(&Server::disconnectHandle, this, std::placeholders::_1));
    // add connection to connections
    connections[newConn->get_conn_id()] = std::move(newConn);
    connections[newConn->get_conn_id()]->ConnectionEstablished();
}

void Server::disconnectHandle(const std::shared_ptr<Connection>& conn) {
    // remove connection from connections
    // std::printf("disconnect %d connection\n", conn_id);
    int conn_id = conn->get_conn_id();
    if (connections.find(conn_id) != connections.end()) {
        connections.erase(conn_id);
    }

    conn->get_epoll_run()->add_to_do(std::bind(&Connection::ConnectionConstructor, conn));
    // printf("disconnect connection finish\n");
    //delete connection
}

void Server::bind_on_connect(std::function<void(std::shared_ptr<Connection>)> func) {
    on_connect_ = std::move(func);
}

void Server::bind_on_message(std::function<void(std::shared_ptr<Connection>)> func) {
    on_message_ = std::move(func);
}
