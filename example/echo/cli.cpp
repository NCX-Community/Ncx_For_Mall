#include "util.h"
#include "endpoint.h"
#include "merror.h"
#include "socket.h"
#include <unistd.h>

const char MSG[] = "hello i am nvxc!";


void connect_to_server(InetAddress remote_endpoint, int cli_fd) {
}

const char SERVER_IP[] = "0.0.0.0";
const uint16_t SERVER_PORT = 6667;
const char CLI_OP[] = "127.0.0.1";
const uint16_t CLI_PORT = 0;

int main() {
    InetAddress remote_endpoint(SERVER_IP, SERVER_PORT);
    InetAddress client_endpoint(CLI_OP, CLI_PORT);
    TcpSocket* cli = new TcpSocket(true); // 非阻塞socket
    cli->bind(client_endpoint);

    // 连接服务器
    socklen_t len = sizeof(remote_endpoint.addr);
    int result = connect(cli->get_fd(), (sockaddr*)&remote_endpoint.addr, len);
    
    if (result < 0 && errno == EINPROGRESS) {
        // 使用select等待连接完成
        fd_set write_fds;
        FD_ZERO(&write_fds);
        FD_SET(cli->get_fd(), &write_fds);
        
        struct timeval timeout;
        timeout.tv_sec = 5;  // 5秒超时
        timeout.tv_usec = 0;
        
        result = select(cli->get_fd() + 1, NULL, &write_fds, NULL, &timeout);
        
        if (result > 0) {
            // 检查连接是否成功
            int error = 0;
            socklen_t len = sizeof(error);
            if (getsockopt(cli->get_fd(), SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error) {
                printf("Connection failed: %s\n", strerror(error));
                return -1;
            }
        } else if (result == 0) {
            printf("Connection timeout\n");
            return -1;
        } else {
            printf("Select error: %s\n", strerror(errno));
            return -1;
        }
    }

    printf("Connection established\n");

    // 发送消息
    if (send(cli->get_fd(), MSG, sizeof(MSG), 0) < 0) {
        printf("Send error: %s\n", strerror(errno));
        return -1;
    }
    printf("Send\n");

    // 非阻塞接收消息
    while(true) {
        char buf[1024];
        bzero(buf, sizeof(buf));
        
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(cli->get_fd(), &read_fds);
        
        struct timeval timeout;
        timeout.tv_sec = 1;  // 1秒超时
        timeout.tv_usec = 0;
        
        result = select(cli->get_fd() + 1, &read_fds, NULL, NULL, &timeout);
        
        if (result > 0) {
            ssize_t bytes_read = read(cli->get_fd(), buf, sizeof(buf));
            if (bytes_read > 0) {
                printf("Message from server: %s\n", buf);
                break;
            } else if (bytes_read == 0) {
                printf("Server closed connection\n");
                break;
            } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
                printf("Read error: %s\n", strerror(errno));
                break;
            }
        } else if (result == 0) {
            continue;  // 超时，继续等待
        } else {
            printf("Select error: %s\n", strerror(errno));
            break;
        }
    }

    delete cli;
    return 0;
}