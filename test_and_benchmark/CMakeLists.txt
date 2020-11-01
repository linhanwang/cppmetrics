add_executable(histogram.test histogram.test.cpp)
target_link_libraries(histogram.test PUBLIC metrics ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY} gflags glog Threads::Threads)

add_test(NAME histogram.test COMMAND histogram.test)

add_executable(histogram.benchmark histogram.benchmark.cpp)
target_link_libraries(histogram.benchmark PUBLIC metrics gflags glog benchmark Threads::Threads)


add_executable(counter.test counter.test.cpp)
target_link_libraries(counter.test PUBLIC metrics ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY} gflags glog Threads::Threads)

add_test(NAME counter.test COMMAND counter.test)


add_executable(gauge.test gauge.test.cpp)
target_link_libraries(gauge.test PUBLIC metrics ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY} gflags glog Threads::Threads)

add_test(NAME gauge.test COMMAND gauge.test)

add_executable(metrics.test metrics.test.cpp)
target_link_libraries(metrics.test PUBLIC metrics ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY} gflags glog Threads::Threads)

add_test(NAME metrics.test COMMAND metrics.test)

add_executable(metrics.benchmark metrics.benchmark.cpp)
target_link_libraries(metrics.benchmark PUBLIC metrics gflags glog benchmark Threads::Threads)