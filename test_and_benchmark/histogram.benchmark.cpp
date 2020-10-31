#include <benchmark/benchmark.h>

#include <metrics/histogram.h>

using namespace metrics;

static void BM_AddValue(benchmark::State& state) {
  Histogram h(1, 0, 100);

  int counter{0};
  for (auto _ : state) {
    ++counter;
    if (counter > 100) counter = 0;

    h.addValue(counter);
  }

  benchmark::DoNotOptimize(h.getAvg());
}

static void BM_GetPercentile100(benchmark::State& state) {
  Histogram h(1, 0, 100);

  for (int i = 0; i < 100; ++i) {
    h.addValue(i);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(h.getPercentileEstimate(0.95));
  }
}

static void BM_GetPercentile10(benchmark::State& state) {
  Histogram h(10, 0, 100);

  for (int i = 0; i < 100; ++i) {
    h.addValue(i);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(h.getPercentileEstimate(0.95));
  }
}

// Register the function as a benchmark
BENCHMARK(BM_AddValue);
BENCHMARK(BM_GetPercentile10);
BENCHMARK(BM_GetPercentile100);

// Run the benchmark
BENCHMARK_MAIN();