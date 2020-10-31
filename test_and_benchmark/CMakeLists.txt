add_executable(histogram.test histogram.test.cpp)
target_link_libraries(histogram.test PUBLIC metrics ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY} gflags glog Threads::Threads)

add_test(NAME histogram.test COMMAND histogram.test)

add_executable(histogram.benchmark histogram.benchmark.cpp)
target_link_libraries(histogram.benchmark PUBLIC metrics gflags glog benchmark Threads::Threads)