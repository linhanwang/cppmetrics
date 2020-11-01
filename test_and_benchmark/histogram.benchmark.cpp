#include <benchmark/benchmark.h>
#include <metrics/histogram.h>

using namespace metrics;

static void BM_AddValue10(benchmark::State& state) {
  Histogram h(1000, 0, 10'000);

  int counter{10};
  for (auto _ : state) {
    h.addValue(counter++);
  }

  benchmark::DoNotOptimize(h.getAvg());
}

static void BM_AddValue20(benchmark::State& state) {
  Histogram h(1000, 0, 20'000);

  int counter{0};
  for (auto _ : state) {
    h.addValue(counter++);
  }

  benchmark::DoNotOptimize(h.getAvg());
}

static void BM_AddValue100(benchmark::State& state) {
  Histogram h(1000, 0, 100'000);

  int counter{0};
  for (auto _ : state) {
    h.addValue(counter++);
  }

  benchmark::DoNotOptimize(h.getAvg());
}

static void BM_GetPercentile10(benchmark::State& state) {
  Histogram h(1000, 0, 10'000);

  for (int i = 0; i < 10'000; ++i) {
    h.addValue(i);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(h.getPercentileEstimate(0.95));
  }
}

static void BM_GetPercentile20(benchmark::State& state) {
  Histogram h(1000, 0, 20'000);

  for (int i = 0; i < 20'000; ++i) {
    h.addValue(i);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(h.getPercentileEstimate(0.95));
  }
}

static void BM_GetPercentile20_Bad(benchmark::State& state) {
  Histogram h(1000, 0, 20'000);

  for (int i = 0; i < 20'000; ++i) {
    h.addValue(i);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(h.getPercentileEstimate(0.50));
  }
}

static void BM_GetPercentile100(benchmark::State& state) {
  Histogram h(1000, 0, 100'000);

  for (int i = 0; i < 100'000; ++i) {
    h.addValue(i);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(h.getPercentileEstimate(0.95));
  }
}

static void BM_GetAvg10(benchmark::State& state) {
  Histogram h(1000, 0, 10'000);

  for (int i = 0; i < 10'000; ++i) {
    h.addValue(i);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(h.getAvg());
  }
}

static void BM_GetAvg20(benchmark::State& state) {
  Histogram h(1000, 0, 20'000);

  for (int i = 0; i < 20'000; ++i) {
    h.addValue(i);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(h.getAvg());
  }
}

static void BM_GetAvg100(benchmark::State& state) {
  Histogram h(1000, 0, 100'000);

  for (int i = 0; i < 100'000; ++i) {
    h.addValue(i);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(h.getAvg());
  }
}

// Register the function as a benchmark
BENCHMARK(BM_AddValue10);
BENCHMARK(BM_AddValue20);
BENCHMARK(BM_AddValue100);
BENCHMARK(BM_GetPercentile10);
BENCHMARK(BM_GetPercentile20);
BENCHMARK(BM_GetPercentile20_Bad);
BENCHMARK(BM_GetPercentile100);
BENCHMARK(BM_GetAvg10);
BENCHMARK(BM_GetAvg20);
BENCHMARK(BM_GetAvg100);

// Run the benchmark
BENCHMARK_MAIN();