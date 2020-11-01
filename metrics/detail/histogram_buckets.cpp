#include <glog/logging.h>
#include <metrics/detail/histogram_buckets.h>

namespace metrics {
namespace detail {

HistogramBuckets::HistogramBuckets(ValueType bucketSize, ValueType min,
                                   ValueType max,
                                   const BucketType& defaultBucket)
    : bucketSize_(bucketSize), min_(min), max_(max) {
  CHECK_GT(bucketSize_, ValueType(0));
  CHECK_LT(min_, max_);

  // Deliberately make this a signed type, because we're about
  // to compare it against max-min, which is nominally signed, too.
  auto numBuckets = std::uint64_t((max - min) / bucketSize);
  // Round up if the bucket size does not fit evenly
  if (numBuckets * bucketSize < max - min) {
    ++numBuckets;
  }
  // Add 2 for the extra 'below min' and 'above max' buckets
  numBuckets += 2;
  buckets_.assign(std::size_t(numBuckets), defaultBucket);
}

std::size_t HistogramBuckets::getBucketIdx(ValueType value) const {
  if (value < min_) {
    return 0;
  } else if (value >= max_) {
    return buckets_.size() - 1;
  } else {
    // the 1 is the below_min bucket
    return std::size_t(((value - min_) / bucketSize_) + 1);
  }
}

double HistogramBuckets::computeAvg() const {
  std::uint64_t count = 0;
  for (std::size_t n = 0; n < buckets_.size(); ++n) {
    count += buckets_[n].count;
  }
  if (count == 0) return 0;

  ValueType sum = 0;
  for (std::size_t n = 0; n < buckets_.size(); ++n) {
    sum += buckets_[n].sum;
  }

  return static_cast<double>(sum) / static_cast<double>(count);
}

std::size_t HistogramBuckets::getPercentileBucketIdx(double pct, double* lowPct,
                                                     double* highPct) const {
  CHECK_GE(pct, 0.0);
  CHECK_LE(pct, 1.0);

  auto numBuckets = buckets_.size();

  // Compute the counts in each bucket
  std::uint64_t totalCount = 0;
  for (std::size_t n = 0; n < numBuckets; ++n) {
    totalCount += buckets_[n].count;
  }

  // If there are no elements, just return the lowest bucket.
  // Note that we return bucket 1, which is the first bucket in the
  // histogram range; bucket 0 is for all values below min_.
  if (totalCount == 0) {
    // Set lowPct and highPct both to 0.
    // getPercentileEstimate() will recognize this to mean that the histogram
    // is empty.
    if (lowPct) {
      *lowPct = 0.0;
    }
    if (highPct) {
      *highPct = 0.0;
    }
    return 1;
  }

#ifndef METRICS_REVERSE_SEARCH
#define METRICS_REVERSE_SEARCH 1
#endif

  if (METRICS_REVERSE_SEARCH) {
    // Loop through all the buckets, keeping track of each bucket's
    // percentile range: [0,10], [10,17], [17,45], etc.  When we find a range
    // that includes our desired percentile, we return that bucket index.
    double prevPct = 1.0;
    double curPct = 1.0;
    std::uint64_t curCount = 0;
    std::size_t idx;
    for (idx = numBuckets - 1; idx >= 0; --idx) {
      if (buckets_[idx].count == 0) {
        // skip empty buckets
        continue;
      }

      prevPct = curPct;
      curCount += buckets_[idx].count;
      curPct = 1 - static_cast<double>(curCount) / totalCount;
      if (pct >= curPct) {
        // This is the desired bucket
        break;
      }
    }

    if (lowPct) {
      *lowPct = curPct;
    }
    if (highPct) {
      *highPct = prevPct;
    }

    return idx;
  } else {
    // Loop through all the buckets, keeping track of each bucket's
    // percentile range: [0,10], [10,17], [17,45], etc.  When we find a range
    // that includes our desired percentile, we return that bucket index.
    double prevPct = 0.0;
    double curPct = 0.0;
    std::uint64_t curCount = 0;
    std::size_t idx;
    for (idx = 0; idx < numBuckets; ++idx) {
      if (buckets_[idx].count == 0) {
        // skip empty buckets
        continue;
      }

      prevPct = curPct;
      curCount += buckets_[idx].count;
      curPct = static_cast<double>(curCount) / totalCount;
      if (pct <= curPct) {
        // This is the desired bucket
        break;
      }
    }

    if (lowPct) {
      *lowPct = prevPct;
    }
    if (highPct) {
      *highPct = curPct;
    }

    return idx;
  }
}

HistogramBuckets::ValueType HistogramBuckets::getPercentileEstimate(
    double pct) const {
  // Find the bucket where this percentile falls
  double lowPct;
  double highPct;
  size_t bucketIdx = getPercentileBucketIdx(pct, &lowPct, &highPct);
  if (lowPct == 0.0 && highPct == 0.0) {
    // Invalid range -- the buckets must all be empty
    // Return the default value for ValueType.
    return ValueType();
  }
  if (lowPct == highPct) {
    // Unlikely to have exact equality,
    // but just return the bucket average in this case.
    // We handle this here to avoid division by 0 below.
    return avgFromBucket(buckets_[bucketIdx]);
  }

  CHECK_GE(pct, lowPct);
  CHECK_LE(pct, highPct);
  CHECK_LT(lowPct, highPct);

  // Compute information about this bucket
  ValueType avg = avgFromBucket(buckets_[bucketIdx]);
  ValueType low;
  ValueType high;
  if (bucketIdx == 0) {
    if (kIsExact && min_ < avg) {
      // This normally shouldn't happen.  This bucket is only supposed to track
      // values less than min_.  Most likely this means that integer overflow
      // occurred, and the code in avgFromBucket() returned a huge value
      // instead of a small one.  Just return the minimum possible value for
      // now.
      //
      // (Note that if the counter keeps being decremented, eventually it will
      // wrap and become small enough that we won't detect this any more, and
      // we will return bogus information.)
      LOG(ERROR) << "invalid average value in histogram minimum bucket: " << avg
                 << " > " << min_ << ": possible integer overflow?";
      return getBucketMin(bucketIdx);
    }
    // For the below-min bucket, just assume the lowest value ever seen is
    // twice as far away from min_ as avg.
    high = min_;
    low = high - (2 * (high - avg));
    // Adjust low in case it wrapped
    if (low > avg) {
      low = std::numeric_limits<ValueType>::min();
    }
  } else if (bucketIdx == buckets_.size() - 1) {
    if (kIsExact && avg < max_) {
      // Most likely this means integer overflow occurred.  See the comments
      // above in the minimum case.
      LOG(ERROR) << "invalid average value in histogram maximum bucket: " << avg
                 << " < " << max_ << ": possible integer overflow?";
      return getBucketMax(bucketIdx);
    }
    // Similarly for the above-max bucket, assume the highest value ever seen
    // is twice as far away from max_ as avg.
    low = max_;
    high = low + (2 * (avg - low));
    // Adjust high in case it wrapped
    if (high < avg) {
      high = std::numeric_limits<ValueType>::max();
    }
  } else {
    low = getBucketMin(bucketIdx);
    high = getBucketMax(bucketIdx);
    if (kIsExact && (avg < low || avg > high)) {
      // Most likely this means an integer overflow occurred.
      // See the comments above.  Return the midpoint between low and high
      // as a best guess, since avg is meaningless.
      LOG(ERROR) << "invalid average value in histogram bucket: " << avg
                 << " not in range [" << low << ", " << high
                 << "]: possible integer overflow?";
      return (low + high) / 2;
    }
  }

  // Since we know the average value in this bucket, we can do slightly better
  // than just assuming the data points in this bucket are uniformly
  // distributed between low and high.
  //
  // Assume that the median value in this bucket is the same as the average
  // value.
  double medianPct = (lowPct + highPct) / 2.0;
  if (pct < medianPct) {
    // Assume that the data points lower than the median of this bucket
    // are uniformly distributed between low and avg
    double pctThroughSection = (pct - lowPct) / (medianPct - lowPct);
    return ValueType(low + ((avg - low) * pctThroughSection));
  } else {
    // Assume that the data points greater than the median of this bucket
    // are uniformly distributed between avg and high
    double pctThroughSection = (pct - medianPct) / (highPct - medianPct);
    return ValueType(avg + ((high - avg) * pctThroughSection));
  }
}

}  // namespace detail

}  // namespace metrics