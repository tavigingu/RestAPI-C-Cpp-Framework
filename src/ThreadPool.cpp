#include "ThreadPool.hpp"

ThreadPool::ThreadPool(size_t num_threads)
{
    for(size_t i=0; i<num_threads; ++i)
        create_thread();
}

ThreadPool::~ThreadPool()
{
    {   
        //lock the queue to update stop flag
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_stopFlag = true;
    }

    //notify all threads so they will wake up and stop
    m_condition.notify_all();

    //wait for each worker thread to finish execution 
    //before destroying the thread pool
    for (auto& thread : m_threads) {
        thread.join();
    }
}

void ThreadPool::enqueue(std::function<void()> task)
{
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_tasks.emplace(move(task));
    }
    m_condition.notify_all();
}

void ThreadPool::create_thread()
{
    m_threads.emplace_back([this] {
            printf("Thread %ld started.\n", std::this_thread::get_id());
            while(true)
            {
                std::function<void()> task; //can store any void function withour parameters

                {
                    //lock the queue so that data can be shared safely
                    std::unique_lock<std::mutex> lock(m_queueMutex); 

                    //waiting until there is a task to execute
                    //or the pool is stopped
                    m_condition.wait(lock, [this] { 
                        return !m_tasks.empty() || m_stopFlag;
                    });

                    //exit the thread if the pool is stopped
                    // and there are no tasks
                    if(m_stopFlag && m_tasks.empty()) {
                        return;
                    }

                    task = std::move(m_tasks.front());
                    m_tasks.pop();

                }//the lock is automatically released here

                task();
            } 
          
        });
}
