#include "epoll_run.h"
#include "epoll.h"
#include "channel.h"
#include "threadPool.h"

EpollRun::EpollRun()
{
    ep = new Epoll();
    quit = false;
}

EpollRun::~EpollRun()
{
    delete ep;
}

void EpollRun::run()
{
    while (!quit)
    {
        std::vector<Channel *> activeChannels = ep->poll();
        // std::cout<<"epoll event count: "<< activeChannels.size() <<std::endl;
        if (activeChannels.empty())
            continue;
        for (auto channel : activeChannels)
        {
            // 将callback函数放入线程池中运行
            channel->callHandle();
        }
    }
}

void EpollRun::stop()
{
    quit = true;
}

void EpollRun::updateChannel(Channel *channel)
{
    ep->updateChannel(channel);
}