#include "threadPool.hpp"

#include <iostream>

ThreadPool::ThreadPool()
{
    unsigned int concurent = std::thread::hardware_concurrency();
    this->threads.reserve(concurent);
    for (unsigned int i = 0; i < concurent; ++i)
    {
        this->threads.emplace_back(
            [this]() -> void
            {
                while (true)
                {
                    // variable for task
                    std::function<void()> task;

                    {

                        std::unique_lock<std::mutex> lock(this->mutex);
                        this->variable.wait(lock,
                                            [this]
                                            {
                                                // wait for stop, or wait for task/s to be available
                                                return this->stop == true || this->tasks.size() > 0;
                                            });

                        if (stop == true)
                        {
                            return;
                        }

                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    task();
                }
            });
    }
}

ThreadPool::ThreadPool(bool joinAfter) : ThreadPool()
{
    this->joinAfter = joinAfter;
}

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(this->mutex);
        this->stop = true;
    }

    // notify all threads to stop
    this->variable.notify_all();

    // join all threads
    for (auto &thread : this->threads)
    {
        if (this->joinAfter)
        {
            thread.join();
        }
        else
        {
            thread.detach();
        }
    }
}

void ThreadPool::push(std::function<void()> function)
{
    {
        std::lock_guard<std::mutex> lock(this->mutex);
        this->tasks.push(std::move(function));
    }

    this->variable.notify_all();
}
