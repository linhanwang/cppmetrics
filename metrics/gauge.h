#pragma once

namespace metrics {

class Gauge {
 public:
  typedef double ValueType;

  Gauge() : gauge_(0) {}

  void addValue(ValueType value) { gauge_ = value; }

  void clear() {}

  ValueType getGauge() const { return gauge_; }

 private:
  ValueType gauge_;
};

}  // namespace metrics