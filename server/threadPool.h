#ifndef THREADPOOL_H
#define THREADPOOL_H

#include"util.h"
class ThreadPool {
public:
    ThreadPool();
    ~ThreadPool();
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    /// 实现能返回值的任务函数
    /// 使用函数模板来实现任务添加函数，使得任务可以是任意的函数
    /// 同时使用右值引用和完美转发来避免不必要的拷贝
    /// 使用后置返回类型来简化函数模板的声明
    template<typename F, typename... Args>
    auto add(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasks_mtx;
    std::condition_variable cv; // 条件变量，用于消费者和生产者之间的同步
    bool stop;
};

template<typename F, typename... Args>
auto ThreadPool::add(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;

    // 使用packaged_task来包装任务函数, 因为需要返回值
    // 因为task需要多线程共享，所以使用shared_ptr来管理
    auto task = std::make_shared<std::packaged_task<return_type()>> (
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();
    {   
        /// 生产者
        std::unique_lock<std::mutex> lock(tasks_mtx);
        if(stop) throw std::runtime_error("add to stopped ThreadPool");
        tasks.emplace([task] {(*task)();});
    }
    /// 通知一个消费者来执行任务
    cv.notify_one();
    return res;
}


#endif