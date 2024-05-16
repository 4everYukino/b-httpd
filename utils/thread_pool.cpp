#include "thread_pool.h"

#include "utils.h"

#include <spdlog/spdlog.h>

Thread_Pool::Thread_Pool(int threads)
{
    exit_ = false;

    if (threads == 0) {
        threads = get_threads_count();
    }

    for (int i = 0; i < threads; i++)
        threads_.emplace_back([this] {
            Task* task = nullptr;
            while (true) {
                {
                    std::unique_lock<std::mutex> lock(this->mutex_);

                    // Call lock.unlock() first,
                    // when 'condition_' is notified, judge lambda,
                    // if true, then wakeup and call lock.lock().
                    this->condition_.wait(lock,
                                          [this] {
                                          return this->exit_ || !this->tasks_.empty();
                                          });

                    if (this->exit_ && this->tasks_.empty())
                        return;

                    // The 'tasks_' should not be empty.
                    task = &this->tasks_.front();
                    this->tasks_.pop();
                }

                auto rc = task->run();
                if (rc)
                    task->after_complated();
                else
                    task->after_failed();
            }
        });

    spdlog::info("Threads pool with max threads {} was created.", threads);
}

Thread_Pool::~Thread_Pool()
{
    exit_ = true;

    condition_.notify_all();

    for (auto& i : threads_) {
        i.join();
    }
}

void Thread_Pool::add(Task& task)
{
    std::lock_guard<std::mutex> guard(mutex_);
    tasks_.push(task);
    condition_.notify_one();
}
