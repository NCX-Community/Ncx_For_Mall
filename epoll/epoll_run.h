#ifndef EPOLL_RUN_H
#define EPOLL_RUN_H
#include"util.h"
class Epoll;
class Channel;

class EpollRun {
private:
    Epoll* ep;
    bool quit;
public:
    EpollRun();
    ~EpollRun();
    void run();
    void stop();
    void updateChannel(Channel* channel);
};


#endif