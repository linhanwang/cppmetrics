add_library(metrics SHARED detail/histogram_buckets.cpp config.cpp)
target_link_libraries(metrics gflags_nothreads glog)