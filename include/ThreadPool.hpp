#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <future>
#include <atomic>

class ThreadPool {
public:
    ThreadPool(int numThreads);
    ~ThreadPool();

    void wait(); // Wait for all tasks to complete

    // Add a task to the thread pool
    template<typename F, typename... Args> 
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::invoke_result<F, Args...>::type> {
using return_type = decltype(f(args...));

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();
    
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks_in_progress++;  // increment before enqueuing

        taskQueue.emplace([this, task]() {
            (*task)();
            // decrement after finishing
            {
                std::unique_lock<std::mutex> lock(task_done_mutex);
                tasks_in_progress--;
            }
            task_done_cv.notify_one(); // notify waiters
        });
    }
    condition.notify_one();
    return res;
}

private:
    std::vector<std::thread> threads; // Vector of worker threads
    std::queue<std::function<void()>> taskQueue; // Queue of tasks
    std::mutex queueMutex; // Mutex for synchronizing access to the task queue
    std::condition_variable condition; // Condition variable for notifying worker threads
    std::atomic<bool> stop; // Flag to indicate if the pool is stopping

    std::atomic<int> tasks_in_progress{0};
    std::condition_variable task_done_cv;
    std::mutex task_done_mutex;
};