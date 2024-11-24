#include "epoll_run.h"
#include "epoll.h"
#include "channel.h"
#include "threadPool.h"

EpollRun::EpollRun(): poller(std::make_unique<Epoll>()) {}
EpollRun::~EpollRun() {}

void EpollRun::run()
{
    while (true)
    {
        std::vector<Channel *> activeChannels = poller->poll();
        // std::cout<<"epoll event count: "<< activeChannels.size() <<std::endl;
        if (activeChannels.empty())
            continue;
        for (auto channel : activeChannels)
        {
            // 将callback函数放入线程池中运行
            channel->handle_event();
        }
        // 此时handle event已经延长connection的生命周期,不用担心在close时把conntion析构
        do_after_handle_events();
    }
}

void EpollRun::update_channel(Channel *channel)
{
    poller->update_channel(channel);
}

void EpollRun::delete_channel(Channel *channel)
{
    poller->delete_channel(channel);
}

void EpollRun::add_to_do(std::function<void()> func)
{
    std::lock_guard<std::mutex> lock(mtx);
    to_do_list.emplace_back(std::move(func));
}

void EpollRun::do_after_handle_events()
{
    std::vector<std::function<void()>> funcs;
    {
        std::lock_guard<std::mutex> lock(mtx);
        funcs.swap(to_do_list);
    }
    for (auto &func : funcs)
    {
        func();
    }
}