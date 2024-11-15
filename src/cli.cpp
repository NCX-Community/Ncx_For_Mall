#include "net_util/endpoint.h"
#include "error/error.h"
#include "socket/socket.h"
#include <unistd.h>

const char MSG[] = "hello i am nvxc!";


void connect_to_server(Endpoint remote_endpoint, int cli_fd) {
}

const char SERVER_IP[] = "0.0.0.0";
const uint16_t SERVER_PORT = 6666;
const char CLI_OP[] = "127.0.0.1";
const uint16_t CLI_PORT = 0;

int
main(void) {
    Endpoint remote_endpoint(SERVER_IP, SERVER_PORT);
    Endpoint client_endpoint(CLI_OP, CLI_PORT);
    Socket* cli = new TcpSocket();
    cli->bind(client_endpoint);
    socklen_t len = sizeof(remote_endpoint.addr);
    int result = connect(cli->get_fd(), (sockaddr*)&remote_endpoint.addr, len);
    errif(result < 0, "cli connect failed");
    errif(send(cli->get_fd(), MSG, sizeof(MSG), 0) < 0, "cli send error");
}