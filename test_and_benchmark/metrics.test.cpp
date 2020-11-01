#include <gtest/gtest.h>
#include <metrics/metrics.h>

using namespace metrics;

TEST(Metrics, HistogramTest) {
  //  Histogram h(1, 0, 1000);

  for (unsigned int n = 0; n < 1000; ++n) {
    GetHisogram(HistogramTest).addValue(n);
  }

  for (unsigned int n = 0; n <= 1000; ++n) {
    double pct = n / 1000.0;

    // Check getPercentileEstimate()
    EXPECT_TRUE(
        std::abs(n - GetHisogram(HistogramTest).getPercentileEstimate(pct)) <=
        1);
  }

  EXPECT_EQ(499, GetHisogram(HistogramTest).getAvg());

  GetHisogram(HistogramTest).clear();

  EXPECT_EQ(0, GetHisogram(HistogramTest).getPercentileEstimate(0.95));
  EXPECT_EQ(0, GetHisogram(HistogramTest).getAvg());
}

TEST(Metrics, CounterTest) {
  for (int i = 0; i < 10; ++i) {
    GetCounter(CounterTest).addValue(i);
  }

  EXPECT_EQ(45, GetCounter(CounterTest).getCounter());

  GetCounter(CounterTest).clear();

  EXPECT_EQ(0, GetCounter(CounterTest).getCounter());
}

TEST(Metrics, GaugeTest) {
  for (int i = 0; i < 10; ++i) {
    GetGauge(GaugeTest).addValue(i);
  }

  EXPECT_EQ(9, GetGauge(GaugeTest).getGauge());

  GetGauge(GaugeTest).clear();
  EXPECT_EQ(9, GetGauge(GaugeTest).getGauge());
}