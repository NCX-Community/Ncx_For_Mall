#ifndef EPOLL_RUN_H
#define EPOLL_RUN_H
#include"util.h"
class Epoll;
class Channel;

class EpollRun {
private:
    Epoll* ep;
    std::unique_ptr<ThreadPool> tp;
    bool quit;
public:
    EpollRun();
    ~EpollRun();
    void run();
    void stop();
    void updateChannel(Channel* channel);
    void addTask(std::function<void()> task);
};


#endif