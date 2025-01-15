#include "epThreadPool.h"
#include "evPoolThread.h"

EpThreadPool::EpThreadPool(EventLoop *main_reactor) : main_reactor_(main_reactor), next_(0)
{
    thread_num_ = std::thread::hardware_concurrency();
}

// 由于线程池内的线程的函数都是循环，所以不需要在析构函数中做额外的停止工作，直接释放资源即可
EpThreadPool::~EpThreadPool() {};

void EpThreadPool::Start() {
    // 创建并启动线程
    for(int i = 0; i < thread_num_; i++) {
        threads_.push_back(std::make_unique<EvPoolThread>());
        loops_.push_back(threads_[i]->StartLoop());
    }
}

void EpThreadPool::set_thread_num(int num) {thread_num_ = num;}

EventLoop* EpThreadPool::NextLoop() {
    // 轮询法获取下一个epoll pool，如果无可用epoll pool则返回主线程的epoll
    EventLoop* res = main_reactor_;
    if(!loops_.empty()) {
        res = loops_[next_++];
        if(next_ == static_cast<int> (loops_.size())) next_ = 0;
    }
    //printf("find available epollthread\n");
    return res;
}