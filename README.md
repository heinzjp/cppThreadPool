# ThreadPool in Modern C++

A simple and efficient thread pool implementation using C++17 features like `std::thread`, `std::function`, `std::future`, and `std::condition_variable`.

## Features

- Reusable pool of worker threads
- Generic `enqueue()` function for submitting any callable
- Handles return values with `std::future`
- Thread-safe task queue
- Clean shutdown on destruction
- CMake-based build system


## Build Instructions

### Prerequisites

- C++17-compatible compiler (e.g., `g++`, `clang++`)
- [CMake](https://cmake.org/) ≥ 3.16

### Build and Run

```bash
git clone https://github.com/heinzjp/cppThreadPool.git
cd cppThreadPool
mkdir build && cd build
cmake ..
make
./ThreadPoolTest
```

## Example Usage
```
ThreadPool pool(4);

auto f1 = pool.enqueue([] {
    std::cout << "Hello from thread pool!" << std::endl;
});

auto f2 = pool.enqueue([](int a, int b) -> int {
    return a + b;
}, 5, 7);

f1.get(); // Waits for completion
std::cout << "5 + 7 = " << f2.get() << std::endl;
```