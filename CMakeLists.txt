cmake_minimum_required(VERSION 3.10)
project(CppMetrics)

set(CMAKE_CXX_STANDARD 17)

## Setup conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# find pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})

# Setup testing
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_subdirectory(metrics)
add_subdirectory(test_and_benchmark)
