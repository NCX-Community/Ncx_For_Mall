// 动态分配socket接口
#include"socket.h"
#include "merror.h"
#include "endpoint.h"

TcpSocket::TcpSocket(bool is_nonblock): is_nonblock_(is_nonblock) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(is_nonblock_) {
        int flags = fcntl(fd, F_GETFL, 0);
        errif(flags == -1, "fcntl error");
        flags |= O_NONBLOCK;
        errif(fcntl(fd, F_SETFL, flags) == -1, "fcntl error");
    }
    //std::cout<<"serv fd: "<<fd<<std::endl;
}

TcpSocket::TcpSocket(int _fd): fd(_fd) {}

TcpSocket::~TcpSocket() {
    printf("close fd: %d\n", fd);
    ::close(fd);
};

void TcpSocket::bind(Endpoint local) {
    errif(::bind(fd, (sockaddr*)&local.addr, sizeof(local.addr)), "tcp bind error");
}

void TcpSocket::connect(Endpoint remote) {
    if( is_nonblock_) {
        errif(connect_on_nonblocking(remote) == false, "tcp connect error!");
    }
    errif(::connect(fd, (sockaddr*)&remote.addr, sizeof(remote.addr)), "tcp connect error");
}

void TcpSocket::listen(int backlog) {
    errif(::listen(fd, backlog), "tcp listen error");
}

int TcpSocket::accept(Endpoint &client) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    bzero(&client_addr, client_addr_len);

    int client_fd = ::accept(fd, (struct sockaddr*)&client_addr, &client_addr_len);
    // 将客户端地址信息存储到 Endpoint 对象中
    client.addr = client_addr;
    return client_fd;
}

int TcpSocket::get_fd() {return fd;}

bool TcpSocket::connect_on_nonblocking(Endpoint remote_endpoint) {
 // 连接服务器
    socklen_t len = sizeof(remote_endpoint.addr);
    int result = ::connect(fd, (sockaddr *)&remote_endpoint.addr, len);

    if (result < 0 && errno == EINPROGRESS)
    {
        // 使用select等待连接完成
        fd_set write_fds;
        FD_ZERO(&write_fds);
        FD_SET(fd, &write_fds);

        struct timeval timeout;
        timeout.tv_sec = 5; // 5秒超时
        timeout.tv_usec = 0;

        result = select(fd + 1, NULL, &write_fds, NULL, &timeout);

        if (result > 0)
        {
            // 检查连接是否成功
            int error = 0;
            socklen_t len = sizeof(error);
            if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error)
            {
                printf("Connection failed: %s\n", strerror(error));
                return -1;
            }
        }
        else if (result == 0)
        {
            printf("Connection timeout\n");
            return -1;
        }
        else
        {
            printf("Select error: %s\n", strerror(errno));
            return -1;
        }
    }

    return true;
}
