#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#define PORT 6666
#define BUFFER_SIZE 1024

int main() {
    // 创建套接字
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket creation failed");
        return -1;
    }

    // 设置地址和端口
    sockaddr_in address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 绑定套接字到端口
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return -1;
    }

    // 开始监听
    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        close(server_fd);
        return -1;
    }

    // 设置非阻塞模式
    int flags = fcntl(server_fd, F_GETFL, 0);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

    std::cout << "HTTP server is running on port " << PORT << "...\n";

    while (true) {
        sockaddr_in client_address;
        socklen_t client_addrlen = sizeof(client_address);

        // 尝试接受客户端连接
        int new_socket = accept(server_fd, (struct sockaddr *)&client_address, &client_addrlen);
        if (new_socket < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // 如果没有连接，继续循环
                usleep(100000); // 睡眠 100 毫秒
                continue;
            } else {
                perror("Accept failed");
                close(server_fd);
                return -1;
            }
        }

        // 设置客户端套接字为非阻塞模式
        flags = fcntl(new_socket, F_GETFL, 0);
        fcntl(new_socket, F_SETFL, flags | O_NONBLOCK);

        char buffer[BUFFER_SIZE] = {0};
        std::string request;

        // 非阻塞读取请求数据
        while (true) {
            int bytes_read = read(new_socket, buffer, BUFFER_SIZE - 1);
            if (bytes_read < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // 如果没有数据可读，稍后再试
                    usleep(100000); // 睡眠 100 毫秒
                    continue;
                } else {
                    perror("Read failed");
                    close(new_socket);
                    break;
                }
            } else if (bytes_read == 0) {
                // 客户端关闭连接
                break;
            } else {
                buffer[bytes_read] = '\0';
                request += buffer;
                // 如果读取到完整请求，退出循环
                if (request.find("\r\n\r\n") != std::string::npos) {
                    break;
                }
            }
        }

        if (!request.empty()) {
            std::cout << "Received request:\n" << request << "\n";

            // 构造 HTTP 响应
            std::string response_header =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: 13\r\n"
                "\r\n";

            std::string response_body = "Hello, World!";

            send(new_socket, response_header.c_str(), response_header.size(), 0);
            send(new_socket, response_body.c_str(), response_body.size(), 0);
        }

        // 关闭客户端连接
        close(new_socket);
    }

    // 关闭服务器套接字
    close(server_fd);
    return 0;
}
