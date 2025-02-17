#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <functional>

/**
 * @brief Thread pool
 */
class ThreadPool
{
    std::vector<std::thread> threads;        ///< Vector of threads
    std::queue<std::function<void()>> tasks; ///< Tasks to be done
    std::mutex mutex;                        ///< Mutex to handle accesing to tasks only frome one thread at the same time
    std::condition_variable variable;        ///< Block threads if no tasks are available
    bool stop = false;                       ///< Variable to stop all threads
    /**
     * @brief Join threads in destructor? ie. wait for threads to complete their work
     * Possible place for memory leak (depends on usecase), if we destruct ThreadPool at the end of the program
     * and joinAfter is false, then the thread could run after the program exited.
     */
    bool joinAfter = true;
    bool completeAllTasks = false; ///< Complete all tasks before destruction?

public:
    ThreadPool(); ///< Constructor
    /**
     * @brief Constructor
     * @param joinAfter Should be threads joined in destructor to wait for their work to be done?
     */
    ThreadPool(bool joinAfter);
    ~ThreadPool(); ///< Destructor

    void setCompleteAllTasks(bool value);

    /**
     * @brief Add new task
     * @param function lambda function to be added
     */
    void push(std::function<void()> function);
};