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
    Server* server = new Server(SERVER_IP, SERVER_PORT, BACKLOG);
    server->start();
    return 0;
}