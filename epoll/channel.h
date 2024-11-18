#ifndef CHANNEL_H  
#define CHANNEL_H
#include<sys/epoll.h>
#include<functional>
#include"epoll.h"
#include"epoll_run.h"

class EpollRun;

class Channel {
private:
    int fd;
    uint32_t evs;
    uint32_t revs;
    EpollRun* er;
    bool is_epolled;
    std::function<void()> handle_func;
public:
    Channel(EpollRun* er, int fd);
    ~Channel();
    void setEpolled();
    void setRevs(uint32_t revs);
    void enableRead();
    uint32_t getEvents();
    int getFd();
    bool isEpolled();
    void setHandleFunc(std::function<void()> func);
    void callHandleFunc();
};

#endif