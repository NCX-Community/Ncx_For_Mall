#include<sys/socket.h>
#include<stdio.h>
#include<cstring>
#include<arpa/inet.h>
#include <unistd.h>

#include"error/error.h"
#include"net_util/endpoint.h"
#include"socket/socket.h"

int create_inet_tcp_server(Endpoint endpoint) {
    int serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(bind(serv_fd, (sockaddr*)&endpoint.addr, sizeof(endpoint.addr)), "tcp bind error!");
    return serv_fd;
}

int run_inet_tcp_server(int serv_fd) {
    puts("server start");
    errif(listen(serv_fd, 10), "tcp listen error!");
    //accept client
    struct sockaddr_in client_addr;
    socklen_t client_ip_len = sizeof(client_addr);
    bzero(&client_addr, client_ip_len);

    int client_fd = accept(serv_fd, (sockaddr*)&client_addr, &client_ip_len);
    errif(client_fd < 0, "tcp accept error!");

    while(true) {
        char buf[1024];
        bzero(buf, sizeof(buf));
        ssize_t recv_len = recv(client_fd, &buf, sizeof(buf), 0);
        if (recv_len > 0) {
            printf("server recv: %s\n", buf);
            break;
        }
        else if (recv_len == 0) {
            printf("client close\n");
            close(client_fd);
        }
        else if (recv_len < 0) {
            errif(true, "tcp client error!");
        }
    }

    close(serv_fd);
    return 0;
}

int 
main(void) {
    char ip[] = "0.0.0.0";
    uint16_t port = 6666;
    Endpoint server_endpoint(ip, port);
    run_inet_tcp_server(create_inet_tcp_server(server_endpoint));
    return 0;
}