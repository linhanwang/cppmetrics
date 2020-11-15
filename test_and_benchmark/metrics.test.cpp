#include <gtest/gtest.h>
#include <metrics/metrics.h>

using namespace metrics;

TEST(Metrics, HistogramTest) {
  //  Histogram h(1, 0, 1000);

  for (int n = 0; n < 1000; ++n) {
    GetHisogram(HistogramTest).addValue(n);
  }

  for (int n = 0; n <= 1000; ++n) {
    double pct = n / 1000.0;

    // Check getPercentileEstimate()
    EXPECT_TRUE(std::abs(int(n - GetHisogram(HistogramTest).getPercentileEstimate(pct))) <= 10);
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

TEST(Metrics, SerializeTest) {
  GetCounter(CounterPlaceOrder).addValue(124324);
  GetCounter(CounterCancelOrder).addValue(124324);
  GetCounter(CounterTest).addValue(124324);

  GetGauge(GaugePnl).addValue(344123.133);
  GetGauge(GaugeTest).addValue(342.234);

  for (int i = 0; i < 1000; ++i) {
    GetHisogram(HistogramMsgLatency).addValue(i);
  }

  for (int i = 0; i < 20'000; ++i) {
    GetHisogram(HistogramTimeUsed).addValue(i);
  }

  for (int i = 0; i < 1000; ++i) {
    GetHisogram(HistogramTest).addValue(i);
  }

  std::string result =
      "{\"hostname\":\"hb14\",\"ServiceName\":\"magic\",\"CoreId\":2,"
      "\"CounterPlaceOrder\":124324,\"CounterCancelOrder\":124324,"
      "\"CounterTest\":124324,\"GaugePnl\":344123.133,\"GaugeTest\":342.234,"
      "\"HistogramMsgLatencyAvg\":499,\"HistogramMsgLatency95\":949,"
      "\"HistogramMsgLatency99\":989,\"HistogramTimeUsedAvg\":9999,"
      "\"HistogramTimeUsed95\":19000,\"HistogramTimeUsed99\":19799,"
      "\"HistogramTestAvg\":499,\"HistogramTest95\":950,\"HistogramTest99\":"
      "990}";

  MetricsData metrics_data;
  metrics_data.counters = {124324, 124324, 124324};
  metrics_data.gauges = {344123, 342.234};
  metrics_data.avgs = {499, 9999, 499};
  metrics_data.v95s = {950, 19000, 950};
  metrics_data.v99s = {989, 19799, 989};

  MetricsData calc_data = GetMetricsData();

  EXPECT_EQ(metrics_data.counters, calc_data.counters);
  EXPECT_EQ(metrics_data.avgs, calc_data.avgs);
  EXPECT_EQ(metrics_data.v95s, calc_data.v95s);
  EXPECT_EQ(metrics_data.v99s, calc_data.v99s);

  for (int i = 0; i < NumGauges; ++i) {
    EXPECT_TRUE((metrics_data.gauges[i] - calc_data.gauges[i]) / (metrics_data.gauges[i] + calc_data.gauges[i]) >
                -1e-3);
    EXPECT_TRUE((metrics_data.gauges[i] - calc_data.gauges[i]) / (metrics_data.gauges[i] + calc_data.gauges[i]) < 1e-3);
  }
}