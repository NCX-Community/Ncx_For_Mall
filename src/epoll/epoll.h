#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
constexpr int MAX_EVENTS = 10;

class Epoll {
private:
    int epoll_fd;
    struct epoll_event events[MAX_EVENTS];
public:
    Epoll();
    ~Epoll();
    void epoll_ctl(int op, int fd, struct epoll_event* ev);
    int get_epfd();
    struct epoll_event* get_events();
    int poll();
};
#endif