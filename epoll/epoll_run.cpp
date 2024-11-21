#include "epoll_run.h"
#include "epoll.h"
#include "channel.h"

EpollRun::EpollRun() {
    ep = new Epoll();
    quit = false;
}

EpollRun::~EpollRun() {
    delete ep;
}

void EpollRun::run() {
    while(!quit) {
        std::cout<<quit<<std::endl;
        std::vector<Channel*> activeChannels = ep->poll();
        // std::cout<<"epoll event count: "<< activeChannels.size() <<std::endl;
        for(auto channel : activeChannels) {
            channel->callHandleFunc();
        }
    }
}

void EpollRun::stop() {
    quit = true;
}

void EpollRun::updateChannel(Channel* channel) {
    ep->updateChannel(channel);
}  
