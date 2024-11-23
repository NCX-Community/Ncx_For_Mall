#include"channel.h"
#include"epoll.h"
#include"epoll_run.h"


Channel::Channel(EpollRun* _er, int _fd): er(_er), fd(_fd), evs(0), revs(0), is_epolled(false){}
Channel::~Channel(){}

void Channel::enableRead() {
    evs |= EPOLLIN | EPOLLPRI;
    er->updateChannel(this); //this like self in rust
}

void Channel::setEpolled() {
    is_epolled = true;
}

void Channel::setET() {
    evs |= EPOLLET;
    er->updateChannel(this);
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

void Channel::setReadHandleFunc(std::function<void()> func) {
    read_handle = func;
}

void Channel::setWriteHandleFunc(std::function<void()> func) {
    write_handle = func;
}

void Channel::callHandle() {
    if(revs & EPOLLIN) read_handle();
    if(revs & EPOLLOUT) write_handle();
}