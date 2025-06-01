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

    // Add a task to the thread pool
    template<typename F, typename... Args> 
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::invoke_result<F, Args...>::type> {
        using return_type = typename std::invoke_result<F, Args...>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (stop) {
                throw std::runtime_error("enqueue on stopped ThreadPool");
            }
            taskQueue.emplace([task]() { (*task)(); });
        }
        condition.notify_one(); // Notify one worker thread that a new task is available
        return res;
}

private:
    std::vector<std::thread> threads; // Vector of worker threads
    std::queue<std::function<void()>> taskQueue; // Queue of tasks
    std::mutex queueMutex; // Mutex for synchronizing access to the task queue
    std::condition_variable condition; // Condition variable for notifying worker threads
    std::atomic<bool> stop; // Flag to indicate if the pool is stopping
};