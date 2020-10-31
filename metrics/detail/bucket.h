#pragma once

#include <cstdint>

namespace metrics {
namespace detail {

struct Bucket {
 public:
  typedef std::uint64_t ValueType;

  Bucket() : sum(ValueType()), count(0) {}

  void clear() {
    sum = ValueType();
    count = 0;
  }

  void add(const ValueType& s, uint64_t c) {
    sum += s;
    count += c;
  }

  Bucket& operator+=(const Bucket& o) {
    add(o.sum, o.count);
    return *this;
  }

  Bucket& operator-=(const Bucket& o) {
    sum -= o.sum;
    count -= o.count;
    return *this;
  }

  ValueType sum;
  uint64_t count;
};

}  // namespace detail

}  // namespace metrics