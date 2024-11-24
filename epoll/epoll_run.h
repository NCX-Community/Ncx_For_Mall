#ifndef EPOLL_RUN_H
#define EPOLL_RUN_H
#include"util.h"
class Epoll;
class Channel;

class EpollRun {
public:
    EpollRun();
    ~EpollRun();
    void run();
    void do_after_handle_events();
    void add_to_do(std::function<void()> func);
    void update_channel(Channel* channel);
    void delete_channel(Channel* channel);
private:
    std::mutex mtx;
    std::vector<std::function<void()>> to_do_list;
    std::unique_ptr<Epoll> poller;
};


#endif