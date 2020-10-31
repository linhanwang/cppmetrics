#include <gtest/gtest.h>
#include <metrics/histogram.h>

using namespace metrics;

// Insert 100 evenly distributed values into a histogram with 100 buckets
TEST(Histogram, Test100) {
  Histogram h(1, 0, 100);

  for (unsigned int n = 0; n < 100; ++n) {
    h.addValue(n);
  }

  for (unsigned int n = 0; n <= 100; ++n) {
    double pct = n / 100.0;

    // Check getPercentileEstimate()
    EXPECT_EQ(n, h.getPercentileEstimate(pct));
  }

  EXPECT_EQ(49, h.getAvg());
}

// Test calling getPercentileBucketIdx() and getPercentileEstimate() on an
// empty histogram
TEST(Histogram, TestEmpty) {
  Histogram h(1, 0, 100);

  for (unsigned int n = 0; n <= 100; ++n) {
    double pct = n / 100.0;

    EXPECT_EQ(0, h.getPercentileEstimate(pct));
    EXPECT_EQ(0, h.getAvg());
  }
}