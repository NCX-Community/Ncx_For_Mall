#include"channel.h"
#include<cstring>

Channel::Channel(EpollRun* _er, int _fd): er(_er), fd(_fd), evs(0), revs(0), is_epolled(false){}
Channel::~Channel(){}

void Channel::enableRead() {
    evs = EPOLLIN | EPOLLET;
    er->updateChannel(this); //this like self in rust
}

void Channel::setEpolled() {
    is_epolled = true;
}

uint32_t Channel::getEvents() {
    return evs;
}

int Channel::getFd() {
    return fd;
}

bool Channel::isEpolled() {
    return is_epolled;
}

void Channel::setRevs(uint32_t _revs) {
    revs = _revs;
}

void Channel::setHandleFunc(std::function<void()> func) {
    handle_func = func;
}

void Channel::callHandleFunc() {
    handle_func();
}