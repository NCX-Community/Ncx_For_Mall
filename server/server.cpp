#include"server.h"


Server::Server(Socket* server, EpollRun* er) {
    this->er = er;
    this->server = server;
    //注册监听连接的channel
    Channel* serverChannel = new Channel(er, server->get_fd());
    serverChannel->setHandleFunc(std::bind(&Server::handleNewConnection, this));
    serverChannel->enableRead();
}

Server::~Server() {}

void Server::handleNewConnection() {
    Endpoint client_addr;
    int client_fd = server->accept(client_addr);

    // create new client channel
    Channel* clientChannel = new Channel(er, client_fd);
    clientChannel->setHandleFunc(std::bind(&Server::handleRead, this, client_fd));
    clientChannel->enableRead();
}

void Server::handleRead(int client_fd) {
    char buf[1024];
    bzero(buf, sizeof(buf));
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(client_fd, buf, sizeof(buf));
        if(bytes_read > 0){
            printf("message from client fd %d: %s\n", client_fd, buf);
            write(client_fd, buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", client_fd);
            close(client_fd);   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}
