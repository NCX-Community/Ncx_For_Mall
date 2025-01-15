#ifndef EPTHREADPOOL_H
#define EPTHREADPOOL_H

#include"util.h"

class EpThreadPool {
public:
    DISALLOW_COPY_AND_MOVE(EpThreadPool);
    EpThreadPool(EventLoop* main_reactor);
    ~EpThreadPool();

    void Start();
    void set_thread_num(int num);
    EventLoop* NextLoop();

private:
    int thread_num_;
    int next_;  // 轮询法返回下一个可用epoll pool;
    EventLoop* main_reactor_;
    std::vector<std::unique_ptr<EvPoolThread>> threads_;
    std::vector<EventLoop*> loops_;
};

#endif