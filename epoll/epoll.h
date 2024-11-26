#ifndef EPOLL_H
#define EPOLL_H

#include"util.h"

class Channel;
constexpr int MAX_EVENTS = 1000;

// use epoll to create and monitor channel 
class Epoll {
public:
    Epoll();
    ~Epoll();
    int epoll_ctl(int op, int fd, struct epoll_event* ev);
    int get_epfd();
    struct epoll_event* get_events();
    std::vector<Channel*> poll();
    void update_channel(Channel* channel);
    void delete_channel(Channel* channel);
private:
    int epoll_fd;
    struct epoll_event events[MAX_EVENTS];
};
#endif