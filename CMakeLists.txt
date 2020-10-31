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

function(compose_test test_name)
    add_executable(${test_name} ${test_name}.cpp)
    target_link_libraries(${test_name} ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY} Threads::Threads)

    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction(compose_test)

function(compose_benchmark bench_name)
    add_executable(${bench_name} ${bench_name}.cpp)
    target_link_directories(${bench_name} benchmark benchmark_main Threads::Threads)
endfunction(compose_benchmark)

add_subdirectory(metrics)
add_subdirectory(test_and_benchmark)
