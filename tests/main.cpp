#include "ThreadPool.hpp"
#include <iostream>

int main() {
    ThreadPool pool(4);

    auto f1 = pool.enqueue([]() {
        std::cout << "Hello from thread!\n";
    });

    auto f2 = pool.enqueue([](int a, int b) {
        return a + b;
    }, 5, 3);

    f1.get();
    std::cout << "Sum: " << f2.get() << std::endl;

    auto f3 = pool.enqueue([]() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Task 3 after sleep completed!\n";
    });

    auto f4 = pool.enqueue([]() {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "Task 4 after sleep completed!\n";
    });

    pool.wait(); // Wait for all tasks to complete
    std::cout << "All tasks completed!\n";

    return 0;
}
