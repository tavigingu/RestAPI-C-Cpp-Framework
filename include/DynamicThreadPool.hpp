#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "ThreadPool.hpp"

class DynamicThreadPool : public ThreadPool
{
public:
    DynamicThreadPool(size_t minThreads=2, size_t maxThreads=8, std::chrono::seconds idleTimeout= std::chrono::seconds(2)) 
        :ThreadPool(minThreads), m_minThreads(minThreads), m_maxThreads(maxThreads), m_idleTimeout(idleTimeout) 
        {   
            m_monitorThread = std::thread(&DynamicThreadPool::monitor, this);
        }
    ~DynamicThreadPool();

    
private:
    void monitor();
    void mark_thread_for_stop();

    size_t m_minThreads;
    size_t m_maxThreads;
    std::chrono::seconds m_idleTimeout;
    std::thread m_monitorThread;
    std::vector<bool> m_threadStopFlags;
};