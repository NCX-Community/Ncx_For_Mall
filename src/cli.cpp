#include "net_util/endpoint.h"
#include "error/error.h"
#include <unistd.h>

const char MSG[] = "hello i am nvxc!";

int create_tcp_cli(Endpoint cli_endpoint) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd < 0, "cli socket create error!");
    socklen_t len = sizeof(cli_endpoint.addr);
    errif(bind(sockfd, (sockaddr*)&cli_endpoint.addr, len), "cli bind error");
    return sockfd;  
}

void connect_to_server(Endpoint remote_endpoint, int cli_fd) {
    socklen_t len = sizeof(remote_endpoint.addr);
    int result = connect(cli_fd, (sockaddr*)&remote_endpoint.addr, len);
    errif(result < 0, "cli connect failed");
    errif(send(cli_fd, MSG, sizeof(MSG), 0) < 0, "cli send error");
    close(cli_fd);
}

const char SERVER_IP[] = "0.0.0.0";
const uint16_t SERVER_PORT = 6666;
const char CLI_OP[] = "127.0.0.1";
const uint16_t CLI_PORT = 0;

int
main(void) {
    Endpoint remote_endpoint(SERVER_IP, SERVER_PORT);
    Endpoint client_endpoint(CLI_OP, CLI_PORT);
    int cli_fd = create_tcp_cli(client_endpoint);
    connect_to_server(remote_endpoint, cli_fd);
}