#include "epoll.h"
#include "error/error.h"
#include <unistd.h>
#include <sys/epoll.h>
#include <cstring>

Epoll::Epoll() {
    epoll_fd = epoll_create1(0);
    errif(epoll_fd < 0, "epoll create error!");
}

Epoll::~Epoll() {
    close(epoll_fd);
    delete[] events;
}

void Epoll::epoll_ctl(int op, int fd, struct epoll_event* ev) {
    ::epoll_ctl(epoll_fd, op, fd, ev);
}

int Epoll::get_epfd() {return epoll_fd;}
struct epoll_event* Epoll::get_events() {return events;}

std::vector<Channel*> Epoll::poll() {
    std::vector<Channel*> recvChannels;
    ::epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    for(auto ev : events) {
        Channel* recvChannel = static_cast<Channel*>(ev.data.ptr);
        recvChannel->setRevs(ev.events);
        recvChannels.push_back(recvChannel);
    }
    return recvChannels;
}

void Epoll::updateChannel(Channel* channel) {
    int recvfd = channel->getFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.events = channel->getEvents();
    ev.data.ptr = channel;
    if(!channel->isEpolled()) {
        epoll_ctl(EPOLL_CTL_ADD, recvfd, &ev);
    }
    else {
        epoll_ctl(EPOLL_CTL_MOD, recvfd, &ev);
    }
}