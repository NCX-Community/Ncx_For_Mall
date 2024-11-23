#include "endpoint.h"
#include "merror.h"
#include "socket.h"
#include <unistd.h>

const char MSG[] = "hello i am nvxc!";


void connect_to_server(Endpoint remote_endpoint, int cli_fd) {
}

const char SERVER_IP[] = "0.0.0.0";
const uint16_t SERVER_PORT = 6667;
const char CLI_OP[] = "127.0.0.1";
const uint16_t CLI_PORT = 0;

int
main() {
    Endpoint remote_endpoint(SERVER_IP, SERVER_PORT);
    Endpoint client_endpoint(CLI_OP, CLI_PORT);
    Socket* cli = new TcpSocket();
    cli->bind(client_endpoint);
    socklen_t len = sizeof(remote_endpoint.addr);
    int result = connect(cli->get_fd(), (sockaddr*)&remote_endpoint.addr, len);
    errif(result < 0, "cli connect failed");
    errif(send(cli->get_fd(), MSG, sizeof(MSG), 0) < 0, "cli send error");
    while(true) {
        char buf[1024];
        bzero(buf, sizeof(buf));
        ssize_t bytes_read = read(cli->get_fd(), buf, sizeof(buf));
        if(bytes_read > 0) {
            printf("message from server: %s\n", buf);
            break;
        } else if(bytes_read == -1 && errno == EINTR) {
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            break;
        } else if(bytes_read == 0) {
            break;
        }
    }
    close(cli->get_fd());
    delete cli;
}