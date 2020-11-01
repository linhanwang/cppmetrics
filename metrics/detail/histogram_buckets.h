#pragma once

#include <glog/logging.h>
#include <metrics/detail/bucket.h>

#include <limits>
#include <vector>

namespace metrics {

namespace detail {

class HistogramBuckets {
 public:
  typedef std::uint64_t ValueType;
  typedef Bucket BucketType;

  HistogramBuckets(ValueType bucketSize, ValueType min, ValueType max,
                   const BucketType& defaultBucket);

  /* Returns the bucket size of each bucket in the histogram. */
  ValueType getBucketSize() const { return bucketSize_; }

  /* Returns the min value at which bucketing begins. */
  ValueType getMin() const { return min_; }

  /* Returns the max value at which bucketing ends. */
  ValueType getMax() const { return max_; }

  /**
   * Returns the number of buckets.
   *
   * This includes the total number of buckets for the [min, max) range,
   * plus 2 extra buckets, one for handling values less than min, and one for
   * values greater than max.
   */
  size_t getNumBuckets() const { return buckets_.size(); }

  /* Returns the bucket index into which the given value would fall. */
  size_t getBucketIdx(ValueType value) const;

  /* Returns the bucket for the specified value */
  BucketType& getByValue(ValueType value) {
    return buckets_[getBucketIdx(value)];
  }

  /* Returns the bucket for the specified value */
  const BucketType& getByValue(ValueType value) const {
    return buckets_[getBucketIdx(value)];
  }

  /**
   * Returns the bucket at the specified index.
   *
   * Note that index 0 is the bucket for all values less than the specified
   * minimum.  Index 1 is the first bucket in the specified bucket range.
   */
  BucketType& getByIndex(size_t idx) { return buckets_[idx]; }

  /* Returns the bucket at the specified index. */
  const BucketType& getByIndex(size_t idx) const { return buckets_[idx]; }

  /**
   * Returns the minimum threshold for the bucket at the given index.
   *
   * The bucket at the specified index will store values in the range
   * [bucketMin, bucketMin + bucketSize), or [bucketMin, max), if the overall
   * max is smaller than bucketMin + bucketSize.
   */
  ValueType getBucketMin(size_t idx) const {
    if (idx == 0) {
      return std::numeric_limits<ValueType>::min();
    }
    if (idx == buckets_.size() - 1) {
      return max_;
    }

    return ValueType(min_ + ((idx - 1) * bucketSize_));
  }

  /**
   * Returns the maximum threshold for the bucket at the given index.
   *
   * The bucket at the specified index will store values in the range
   * [bucketMin, bucketMin + bucketSize), or [bucketMin, max), if the overall
   * max is smaller than bucketMin + bucketSize.
   */
  ValueType getBucketMax(size_t idx) const {
    if (idx == buckets_.size() - 1) {
      return std::numeric_limits<ValueType>::max();
    }

    return ValueType(min_ + (idx * bucketSize_));
  }

  /**
   * Computes the total number of values stored across all buckets.
   *
   * Runs in O(numBuckets)
   *
   * @param countFn A function that takes a const BucketType&, and returns the
   *                number of values in that bucket
   * @return Returns the total number of values stored across all buckets
   */
  double computeAvg() const;

  /**
   * Determine which bucket the specified percentile falls into.
   *
   * Looks for the bucket that contains the Nth percentile data point.
   *
   * @param pct     The desired percentile to find, as a value from 0.0 to 1.0.
   * @param countFn A function that takes a const BucketType&, and returns the
   *                number of values in that bucket.
   * @param lowPct  The lowest percentile stored in the selected bucket will be
   *                returned via this parameter.
   * @param highPct The highest percentile stored in the selected bucket will
   *                be returned via this parameter.
   *
   * @return Returns the index of the bucket that contains the Nth percentile
   *         data point.
   */
  std::size_t getPercentileBucketIdx(double pct,
                                double* lowPct = nullptr,
                                double* highPct = nullptr) const;

  /**
   * Estimate the value at the specified percentile.
   *
   * @param pct     The desired percentile to find, as a value from 0.0 to 1.0.
   * @param countFn A function that takes a const BucketType&, and returns the
   *                number of values in that bucket.
   * @param avgFn   A function that takes a const BucketType&, and returns the
   *                average of all the values in that bucket.
   *
   * @return Returns an estimate for N, where N is the number where exactly pct
   *         percentage of the data points in the histogram are less than N.
   */
  ValueType getPercentileEstimate(double pct) const;

  /*
   * Iterator access to the buckets.
   *
   * Note that the first bucket is for all values less than min, and the last
   * bucket is for all values greater than max.  The buckets tracking values in
   * the [min, max) actually start at the second bucket.
   */
  typename std::vector<BucketType>::const_iterator begin() const {
    return buckets_.begin();
  }
  typename std::vector<BucketType>::iterator begin() {
    return buckets_.begin();
  }
  typename std::vector<BucketType>::const_iterator end() const {
    return buckets_.end();
  }
  typename std::vector<BucketType>::iterator end() { return buckets_.end(); }

 private:
  static ValueType avgFromBucket(const Bucket& bucket) {
    if (bucket.count == 0) {
      return ValueType(0);
    }
    // Cast bucket.count to a signed integer type.  This ensures that we
    // perform division properly here: If bucket.sum is a signed integer
    // type but we divide by an unsigned number, unsigned division will be
    // performed and bucket.sum will be converted to unsigned first.
    // If bucket.sum is unsigned, the code will still do unsigned division
    // correctly.
    //
    // The only downside is if bucket.count is large enough to be negative
    // when treated as signed.  That should be extremely unlikely, though.
    return bucket.sum / static_cast<std::int64_t>(bucket.count);
  }

  static constexpr bool kIsExact = std::numeric_limits<ValueType>::is_exact;
  ValueType bucketSize_;
  ValueType min_;
  ValueType max_;
  std::vector<BucketType> buckets_;
};

}  // namespace detail

} // namespace metrics