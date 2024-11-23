#include "threadPool.h"

ThreadPool::ThreadPool() : stop(false) {
    int threadNum = std::thread::hardware_concurrency();
    for(int i = 0; i < threadNum; i++) {
        threads.emplace_back(std::thread([this] {
            while(true) {
                std::unique_lock<std::mutex> lock(tasks_mtx);
                // 消费者需要等待任务队列中有任务；
                // 如果线程池被关闭或者任务队列非空，就不用等待了
                cv.wait(lock, [this] {return stop || !tasks.empty();});

                if(stop && tasks.empty()) return;
                
                std::function<void()> task = tasks.front();
                tasks.pop();
                lock.unlock();
                task();
            }
        }));
    }
}

// 当cv等待的条件发生变化时，需要手动通知所有的线程
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);
        stop = true;
    }
    cv.notify_all();
    for(auto& thread : threads) {
        if(thread.joinable()) thread.join();
    }
}
