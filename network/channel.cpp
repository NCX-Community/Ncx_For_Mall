#include"channel.h"
#include"epoll.h"
#include"EventLoop.h"
#include"util.h"


Channel::Channel(EventLoop* _er, int _fd): loop_(_er), fd_(_fd), listen_events_(0), ready_events_(0), is_epolled_(false){}
Channel::~Channel(){
    if(fd_ > 0) {close(fd_);}
}

void Channel::enableRead() {
    listen_events_ |= EPOLLIN | EPOLLPRI;
    loop_->update_channel(this); //this like self in rust
}

void Channel::enableWrite() {
    listen_events_ |= EPOLLOUT;
    loop_->update_channel(this);
}

void Channel::disableWrite() {
    listen_events_ &= ~EPOLLOUT;
    loop_->update_channel(this);
}

void Channel::setEpolled(bool in) {
    is_epolled_ = in;
}

void Channel::set_et() {
    listen_events_ |= EPOLLET;
    loop_->update_channel(this);
}

short Channel::listen_events() {
    return listen_events_;
}

short Channel::ready_events() {
    return ready_events_;
}

int Channel::getFd() {
    return fd_;
}

bool Channel::isEpolled() {
    return is_epolled_;
}

void Channel::setRevs(uint32_t _revs) {
    ready_events_ = std::move(_revs);
}

void Channel::set_read_callback(std::function<void()> func) {
    read_callback_ = std::move(func);
    this->enableRead();
}

void Channel::set_write_callback(std::function<void()> func) {
    write_callback_ = std::move(func);
}

void Channel::set_tie(const std::shared_ptr<void>& tie) {
    tie_ = tie;
}

void Channel::handle_event() {
    if(!tie_.expired()) {
        // 延长connection的生命周期，防止在处理事件时connection被析构
        std::shared_ptr<void> guard = tie_.lock();
        handle_event_guard();
    }
    else {
        handle_event_guard();
    }
}

void Channel::handle_event_guard() {
    if(ready_events_ & EPOLLIN) {
        printf("channel read callback\n");
        if(read_callback_) read_callback_();
        else std::puts("channel read callback not init");
    }
    if(ready_events_ & EPOLLOUT) {
        if(write_callback_) write_callback_();
        else std::puts("channel write callback not init");
    }
}


