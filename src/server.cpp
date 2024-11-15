#include <sys/socket.h>
#include <stdio.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#include "error/error.h"
#include "net_util/endpoint.h"
#include "socket/socket.h"
#include "epoll/epoll.h"
#include <cerrno>

static const char SERVER_IP[] = "0.0.0.0";
static const uint16_t SERVER_PORT = 6666;
static const int BACKLOG = 10;


void server_run(Socket *server)
{
    puts("server start");
    // create epoll
    Epoll epoll;
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = server->get_fd();
    epoll.epoll_ctl(EPOLL_CTL_ADD, server->get_fd(), &ev);

    // listen
    server->listen(BACKLOG);
    // while accept client
    while (true)
    {
        int nfds = epoll.poll();
        struct epoll_event *events = epoll.get_events();
        for (int i = 0; i < nfds; i++)
        {
            if (events[i].data.fd == server->get_fd())
            {
                // accept client
                Endpoint client_addr;
                int client_fd = server->accept(client_addr);
                // add client_fd to epoll
                struct epoll_event cli_ev;
                ev.data.fd = client_fd;
                ev.events = EPOLLIN;
                epoll.epoll_ctl(EPOLL_CTL_ADD, client_fd, &cli_ev);
            }
            else if (events[i].events & EPOLLIN)
            {
                // client send message, server should handle
                int client_fd = events[i].data.fd;
                char buf[1024];
                bzero(buf, sizeof(buf));
                while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
                    bzero(&buf, sizeof(buf));
                    ssize_t bytes_read = read(events[i].data.fd, buf, sizeof(buf));
                    if(bytes_read > 0){
                        printf("message from client fd %d: %s\n", events[i].data.fd, buf);
                        write(events[i].data.fd, buf, sizeof(buf));
                    } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
                        printf("continue reading");
                        continue;
                    } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
                        printf("finish reading once, errno: %d\n", errno);
                        break;
                    } else if(bytes_read == 0){  //EOF，客户端断开连接
                        printf("EOF, client fd %d disconnected\n", events[i].data.fd);
                        close(events[i].data.fd);   //关闭socket会自动将文件描述符从epoll树上移除
                        break;
                    }
                }
            }
        }
    }
    return;
}

int main(void)
{
    // create tcp server
    Socket *server = create_socket(Type::TCP);
    Endpoint server_endpoint(SERVER_IP, SERVER_PORT);
    server->bind(server_endpoint);
    server_run(server);
    return 0;
}