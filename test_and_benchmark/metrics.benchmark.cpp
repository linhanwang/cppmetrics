#include <benchmark/benchmark.h>
#include <metrics/metrics.h>

using namespace metrics;

static void BM_HistogramAddValue20(benchmark::State& state) {
  benchmark::DoNotOptimize(GetHisogram(HistogramTimeUsed));

  int counter{0};
  for (auto _ : state) {
    GetHisogram(HistogramTimeUsed).addValue(counter++);
  }

  benchmark::DoNotOptimize(GetHisogram(HistogramTimeUsed).getAvg());
  GetHisogram(HistogramTimeUsed).clear();
}

static void BM_CounterAddValue(benchmark::State& state) {
  benchmark::DoNotOptimize(GetCounter(CounterTest));

  for (auto _ : state) {
    GetCounter(CounterTest).addValue(1);
  }
  benchmark::DoNotOptimize(GetCounter(CounterTest).getCounter());
  GetCounter(CounterTest).clear();
}

static void BM_GaugeAddValue(benchmark::State& state) {
  benchmark::DoNotOptimize(GetGauge(GaugeTest));

  int counter{0};
  for (auto _ : state) {
    GetGauge(GaugeTest).addValue(counter++);
  }
  benchmark::DoNotOptimize(GetGauge(GaugeTest).getGauge());
  GetGauge(GaugeTest).clear();
}

static void BM_HistogramGetPercentile20(benchmark::State& state) {
  for (int i = 0; i < 20'000; ++i) {
    GetHisogram(HistogramTimeUsed).addValue(i);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(GetHisogram(HistogramTimeUsed).getPercentileEstimate(0.95));
  }

  GetHisogram(HistogramTimeUsed).clear();
}

static void BM_HistogramGetPercentile20_Bad(benchmark::State& state) {
  for (int i = 0; i < 20'000; ++i) {
    GetHisogram(HistogramTimeUsed).addValue(i);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(GetHisogram(HistogramTimeUsed).getPercentileEstimate(0.05));
  }

  GetHisogram(HistogramTimeUsed).clear();
}

static void BM_HistogramGetAvg20(benchmark::State& state) {
  for (int i = 0; i < 20'000; ++i) {
    GetHisogram(HistogramTimeUsed).addValue(i);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(GetHisogram(HistogramTimeUsed).getAvg());
  }

  GetHisogram(HistogramTimeUsed).clear();
}

static void BM_Serialize(benchmark::State& state) {
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

  for (auto _ : state) {
    benchmark::DoNotOptimize(GetMetricsData());
  }

  Refresh();
}

BENCHMARK(BM_HistogramAddValue20);
BENCHMARK(BM_CounterAddValue);
BENCHMARK(BM_GaugeAddValue);
BENCHMARK(BM_HistogramGetPercentile20);
BENCHMARK(BM_HistogramGetPercentile20_Bad);
BENCHMARK(BM_HistogramGetAvg20);
BENCHMARK(BM_Serialize);

// Run the benchmark
BENCHMARK_MAIN();