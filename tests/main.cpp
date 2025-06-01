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

    return 0;
}
