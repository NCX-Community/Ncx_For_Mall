#include "util.h"
#include "merror.h"
#include "endpoint.h"
#include "socket.h"
#include "epoll_run.h"
#include "server.h"
#include "common.h"
#include "connection.h"

static const char SERVER_IP[] = "0.0.0.0";
static const uint16_t SERVER_PORT = 6667;
static const int BACKLOG = 10;


int main(void)
{

    EpollRun* main_reactor = new EpollRun();
    Server* server = new Server(main_reactor, SERVER_IP, SERVER_PORT, BACKLOG);
    
    server->start();
    return 0;
}