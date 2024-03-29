#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <functional>

class ThreadPool
{
    std::vector<std::thread> threads;
    std::vector<std::function<void>> tasks;
    std::mutex mutex;
    std::condition_variable variable;

public:
    ThreadPool();
};