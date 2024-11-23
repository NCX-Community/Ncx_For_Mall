#ifndef CHANNEL_H  
#define CHANNEL_H
#include"util.h"

class EpollRun;

class Channel {
private:
    int fd;
    uint32_t evs;
    uint32_t revs;
    EpollRun* er;
    bool is_epolled;
    std::function<void()> read_handle;
    std::function<void()> write_handle;
public:
    Channel(EpollRun* er, int fd);
    ~Channel();
    void setEpolled();
    void setRevs(uint32_t revs);
    void enableRead();
    uint32_t getEvents();
    int getFd();
    bool isEpolled();
    void setReadHandleFunc(std::function<void()> func);
    void setWriteHandleFunc(std::function<void()> func);
    void setET();
    void callHandle();
};

#endif