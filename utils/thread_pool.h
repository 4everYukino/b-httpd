#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include "task.h"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

// TODO:
// * Use core threads strategy

class Thread_Pool
{
public:
    Thread_Pool(int threads = 0);
    ~Thread_Pool();

    void add(Task& task);

private:
    std::atomic<bool> exit_;

    std::mutex mutex_;
    std::condition_variable condition_;

    std::vector<std::thread> threads_;

    std::queue<Task> tasks_;

};

#endif
