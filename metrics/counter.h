#pragma once

#include <cstdint>

namespace metrics {

class Counter {
 public:
  typedef std::int64_t ValueType;

  Counter() : counter_(0) {}

  void addValue(ValueType value) { counter_ += value; }

  void clear() { counter_ = 0; }

  ValueType getCounter() const { return counter_; }

 private:
  ValueType counter_;
};

}  // namespace metrics