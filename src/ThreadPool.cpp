#include "ThreadPool.hpp"

ThreadPool::ThreadPool(int numThreads) : stop(false) {
    for (int i = 0; i < numThreads; ++i) {
        // Create a worker thread and add it to the pool
        threads.emplace_back([this] {
            // Each worker thread runs in an infinite loop waiting for tasks.
            while (true) {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(this->queueMutex);
                    this->condition.wait(lock, [this] { return this->stop || !this->taskQueue.empty(); });
                    if (this->stop && this->taskQueue.empty()) return;
                    task = std::move(this->taskQueue.front());
                    this->taskQueue.pop();
                }

                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    stop = true; // Set the stop flag to true
    condition.notify_all(); // Notify all worker threads to wake up

    for (std::thread &worker : threads) {
        worker.join(); // Wait for all threads to finish
    }
}