#include <sys/socket.h>
#include <stdio.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#include "merror.h"
#include "endpoint.h"
#include "socket.h"
#include "epoll_run.h"
#include "server.h"
#include "common.h"

static const char SERVER_IP[] = "0.0.0.0";
static const uint16_t SERVER_PORT = 6667;
static const int BACKLOG = 10;


int main(void)
{
    // create tcp server
    Socket *serv_sock = create_socket(Type::TCP);
    Endpoint server_endpoint(SERVER_IP, SERVER_PORT);
    serv_sock->bind(server_endpoint);
    serv_sock->listen(BACKLOG);

    // create and run epoll
    EpollRun* epoll_run = new EpollRun();
    Server* server = new Server(serv_sock, epoll_run);
    epoll_run->run();

    return 0;
}