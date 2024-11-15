#include "epoll.h"
#include "error/error.h"
#include <unistd.h>

Epoll::Epoll() {
    epoll_fd = epoll_create1(0);
    errif(epoll_fd < 0, "epoll create error!");
}

Epoll::~Epoll() {
    close(epoll_fd);
}

void Epoll::epoll_ctl(int op, int fd, struct epoll_event* ev) {
    ::epoll_ctl(epoll_fd, op, fd, ev);
}

int Epoll::get_epfd() {return epoll_fd;}
struct epoll_event* Epoll::get_events() {return events;}

int Epoll::poll() {
    return ::epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
}