#include "epoll_run.h"
#include "epoll.h"
#include "channel.h"
#include "current_thread.h"

EpollRun::EpollRun(): poller(std::make_unique<Epoll>()){
    wakeup_fd_ = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    wakeup_channel_ = std::make_unique<Channel>(this, wakeup_fd_);    

    wakeup_channel_->set_read_callback(std::bind(&EpollRun::wakeup_callback, this));
    wakeup_channel_->enableRead();
    wakeup_channel_->setEpolled(true);
}
EpollRun::~EpollRun() {
    // close eventfd channel and eventfd
    poller->delete_channel(wakeup_channel_.get());
    ::close(wakeup_fd_);
}

void EpollRun::run()
{
    // 将实际运行epoll run的线程tid保存在tid中
    tid_ = CURRENT_THREAD::tid();

    while (true)
    {
        std::vector<Channel *> activeChannels = poller->poll();
        // std::cout<<"epoll event count: "<< activeChannels.size() <<std::endl;
        if (activeChannels.empty())
            continue;
        for (auto channel : activeChannels)
        {
            // 将callback函数放入线程池中运行
            channel->handle_event();
        }
        // 此时handle event已经延长connection的生命周期,不用担心在close时把conntion析构
        do_after_handle_events();
    }
}

void EpollRun::update_channel(Channel *channel)
{
    poller->update_channel(channel);
}

void EpollRun::delete_channel(Channel *channel)
{
    poller->delete_channel(channel);
}

void EpollRun::add_to_do(std::function<void()> func)
{
    std::lock_guard<std::mutex> lock(mtx);
    to_do_list.emplace_back(std::move(func));
}

void EpollRun::do_after_handle_events()
{
    std::vector<std::function<void()>> funcs;
    {
        std::lock_guard<std::mutex> lock(mtx);
        funcs.swap(to_do_list);
    }
    for (auto &func : funcs)
    {
        func();
    }
}

bool EpollRun::isInEpollLoop()
{
    return tid_ == CURRENT_THREAD::tid();
}

void EpollRun::run_on_onwer_thread(std::function<void()> cb)
{
    if (isInEpollLoop())cb();
    else add_to_do(std::move(cb));
}

void EpollRun::wakeup_callback() {
    // read
    uint64_t one = 1;
    ssize_t n = ::read(wakeup_fd_, &one, sizeof one);
    if(n != sizeof one) {
        std::cout<<"EpollRun::wakeup_callback() reads "<<n<<" bytes instead of 1"<<std::endl;
    }
    return;
}

int EpollRun::wakeup_fd() {
    return wakeup_fd_;
}