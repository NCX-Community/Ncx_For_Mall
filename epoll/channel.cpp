#include"channel.h"
#include"epoll.h"
#include"epoll_run.h"


Channel::Channel(EpollRun* _er, int _fd): er(_er), fd(_fd), evs(0), revs(0), is_epolled(false), use_threadpool(true){}
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

void Channel::setUseThreadPool(bool use_threadpool) {
    use_threadpool = use_threadpool;
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
    if(revs & EPOLLIN) {
        if(use_threadpool) er->addTask(read_handle);
        else read_handle();
    }

    if(revs & EPOLLOUT) {
        if(use_threadpool) er->addTask(write_handle);
        else write_handle();
    }
}