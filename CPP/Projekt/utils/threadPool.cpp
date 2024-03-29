#include "threadPool.hpp"

#include <iostream>

ThreadPool::ThreadPool()
{
    unsigned int concurent = std::thread::hardware_concurrency();
    this->threads.reserve(concurent);
    for (unsigned int i = 0; i < concurent; ++i)
    {
        this->threads.emplace_back([this]() -> void
                                   { while (true) {
                                    this->variable.wait()
                                   } });
    }
}
