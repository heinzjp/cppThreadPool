cmake_minimum_required(VERSION 3.16)
project(ThreadPoolProject VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directory
include_directories(include)

# Source files
set(SOURCES
    src/ThreadPool.cpp
)

# Header files (not strictly needed for build, but good for IDEs)
set(HEADERS
    include/ThreadPool.hpp
)

# Add executable for tests or usage
add_executable(ThreadPoolTest
    tests/main.cpp
    ${SOURCES}
    ${HEADERS}
)

# Optional: warnings
target_compile_options(ThreadPoolTest PRIVATE -Wall -Wextra -pedantic)
