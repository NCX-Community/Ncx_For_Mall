#ifndef CHANNEL_H  
#define CHANNEL_H
#include"util.h"

class EventLoop;

class Channel {
public:
    DISALLOW_MOVE(Channel);

    Channel(EventLoop* er, int fd);
    ~Channel();

    void enableRead();
    void enableWrite();
    void disableWrite();
    void disableAll() { listen_events_ = 0; loop_->update_channel(this); }

    int getFd();
    short listen_events();
    short ready_events();
    void setRevs(uint32_t revs);

    bool isEpolled();
    void setEpolled(bool in = true);

    void set_read_callback(std::function<void()> func);
    void set_write_callback(std::function<void()> func);
    void set_et();
    void set_tie(const std::shared_ptr<void>& tie);

    void handle_event();
    void handle_event_guard();

    void remove() { loop_->delete_channel(this); }

private:
    int fd_;
    EventLoop* loop_;
    std::weak_ptr<void> tie_; 
    
    short listen_events_;
    short ready_events_;

    bool is_epolled_{false};

    std::function<void()> read_callback_;
    std::function<void()> write_callback_;
};

#endif