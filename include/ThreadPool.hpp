#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool 
{
public:
    ThreadPool(size_t num_threads=std::thread::hardware_concurrency()/2); // initializes with the number of logical CPU cores by default
    virtual ~ThreadPool();

    void enqueue(std::function<void()> task);
    
protected:
    void create_thread();
    void worker_thread();
    void resize_pool(size_t targetSize);
    void check_and_scale_up();


    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_queueMutex;
    std::condition_variable m_condition;
    std::atomic<bool> m_stopFlag;
};