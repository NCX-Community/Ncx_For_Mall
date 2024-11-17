#include "epoll_run.h"

EpollRun::EpollRun() {
    ep = new Epoll();
    quit = false;
}

EpollRun::~EpollRun() {
    delete ep;
}

void EpollRun::run() {
    while(!quit) {
        std::vector<Channel*> activeChannels = ep->poll();
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
