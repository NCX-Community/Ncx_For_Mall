#ifndef EPOLL_RUN_H
#define EPOLL_RUN_H
#include"util.h"
class Epoll;
class Channel;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    void run();

    void update_channel(Channel* channel);
    void delete_channel(Channel* channel);

    void run_on_onwer_thread(std::function<void()> cb);


    int wakeup_fd();
    void wakeup_loop(); //wake up function

    bool isInEpollLoop();
private:
    void add_to_do(std::function<void()> func);
    void do_after_handle_events();
    void wakeup_callback();

    pid_t tid_;
    std::mutex mtx;
    std::vector<std::function<void()>> to_do_list;
    std::unique_ptr<Epoll> poller;

    //eventfd to wake up epoll_wait on closehandle
    int wakeup_fd_;
    std::unique_ptr<Channel> wakeup_channel_;
};


#endif