#pragma once

#include <metrics/detail/histogram_buckets.h>

namespace metrics {

class Histogram {
 public:
  typedef std::int64_t ValueType;
  typedef detail::Bucket Bucket;

  Histogram(ValueType bucketSize, ValueType min, ValueType max)
      : buckets_(bucketSize, min, max, Bucket()) {}

  /* Add a data point to the histogram */
  void addValue(ValueType value) { buckets_.getByValue(value).add(value, 1); }

  /* Remove all data points from the histogram */
  void clear() {
    for (size_t i = 0; i < buckets_.getNumBuckets(); i++) {
      buckets_.getByIndex(i).clear();
    }
  }

  ValueType getAvg() const {
    return static_cast<ValueType>(buckets_.computeAvg());
  }

  /**
   * Estimate the value at the specified percentile.
   *
   * @param pct     The desired percentile to find, as a value from 0.0 to 1.0.
   *
   * @return Returns an estimate for N, where N is the number where exactly pct
   *         percentage of the data points in the histogram are less than N.
   */
  ValueType getPercentileEstimate(double pct) const {
    return buckets_.getPercentileEstimate(pct);
  }

 private:
  detail::HistogramBuckets buckets_;
};

}  // namespace metrics