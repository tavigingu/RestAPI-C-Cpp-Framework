#include "DynamicThreadPool.hpp"

DynamicThreadPool::~DynamicThreadPool()
{
    if (m_monitorThread.joinable()) {
        m_monitorThread.join();
    }
}

void DynamicThreadPool::monitor()
{   
    while(!m_stopFlag)
    {
        std::this_thread::sleep_for(m_idleTimeout);

        std::unique_lock<std::mutex> lock(m_queueMutex);

        if(m_tasks.size() > m_threads.size() && m_threads.size() < m_maxThreads){
            create_thread();
        }
        else if(m_tasks.empty() && m_threads.size() > m_minThreads) {
            m_threads.back().detach();
            m_threads.pop_back();
        }
    }
}

void DynamicThreadPool::mark_thread_for_stop()
{
    for (size_t i = 0; i < m_threadStopFlags.size(); ++i) 
    {
        if (!m_threadStopFlags[i]) {
            m_threadStopFlags[i] = true;  // mark thread for stop
            m_threads[i].join();          // wait thread to finish
            m_threads.erase(m_threads.begin() + i);         // delete thread
            m_threadStopFlags.erase(m_threadStopFlags.begin() + i);  // delete flag
            break;
        }
    }
}
