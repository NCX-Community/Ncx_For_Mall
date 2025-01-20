#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    // 创建 socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // 设置服务器地址
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(7777);
    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return 1;
    }

    // 连接服务器
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return 1;
    }

    std::cout << "Connected to 127.0.0.1:7777" << std::endl;

    // 关闭 socket
    close(sockfd);
    return 0;
}