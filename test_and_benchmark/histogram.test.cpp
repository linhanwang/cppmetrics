#include <gtest/gtest.h>
#include <metrics/histogram.h>

using namespace metrics;

// Insert 100 evenly distributed values into a histogram with 100 buckets
TEST(Histogram, Test1000) {
  Histogram h(1, 0, 1000);

  for (unsigned int n = 0; n < 1000; ++n) {
    h.addValue(n);
  }

  for (unsigned int n = 0; n <= 1000; ++n) {
    double pct = n / 1000.0;

    // Check getPercentileEstimate()
    EXPECT_TRUE(std::abs(n - h.getPercentileEstimate(pct)) <= 1);
  }

  EXPECT_EQ(499, h.getAvg());
}

// Test calling getPercentileBucketIdx() and getPercentileEstimate() on an
// empty histogram
TEST(Histogram, TestEmpty) {
  Histogram h(1, 0, 100);

  for (unsigned int n = 0; n < 1000; ++n) {
    h.addValue(n);
  }

  h.clear();

  for (unsigned int n = 0; n <= 100; ++n) {
    double pct = n / 100.0;

    EXPECT_EQ(0, h.getPercentileEstimate(pct));
    EXPECT_EQ(0, h.getAvg());
  }
}