#include <benchmark/benchmark.h>
#include <metrics/histogram.h>

using namespace metrics;

static void BM_AddValue20(benchmark::State& state) {
  Histogram h(1000, 0, 20'000);

  int counter{0};
  for (auto _ : state) {
    h.addValue(counter++);
  }

  benchmark::DoNotOptimize(h.getAvg());
}

static void BM_AddValue50(benchmark::State& state) {
  Histogram h(1000, 0, 50'000);

  int counter{0};
  for (auto _ : state) {
    h.addValue(counter++);
  }

  benchmark::DoNotOptimize(h.getAvg());
}

static void BM_GetPercentile20Lower(benchmark::State& state) {
  Histogram h(1000, 0, 20'000);

  for (int i = 0; i < 10'000; ++i) {
    h.addValue(i);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(h.getPercentileEstimate(0.95));
  }
}

static void BM_GetPercentile20Higher(benchmark::State& state) {
  Histogram h(1000, 0, 20'000);

  for (int i = 10'000; i < 20'000; ++i) {
    h.addValue(i);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(h.getPercentileEstimate(0.95));
  }
}

static void BM_GetPercentile20WholeRange(benchmark::State& state) {
  Histogram h(1000, 0, 20'000);

  for (int i = 0; i < 20'000; ++i) {
    h.addValue(i);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(h.getPercentileEstimate(0.05));
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

static void BM_GetPercentile50Middle(benchmark::State& state) {
  Histogram h(1000, 0, 50'000);

  for (int i = 20'000; i < 30'000; ++i) {
    h.addValue(i);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(h.getPercentileEstimate(0.95));
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

static void BM_GetAvg50(benchmark::State& state) {
  Histogram h(1000, 0, 50'000);

  for (int i = 0; i < 50'000; ++i) {
    h.addValue(i);
  }

  for (auto _ : state) {
    benchmark::DoNotOptimize(h.getAvg());
  }
}

// Register the function as a benchmark
BENCHMARK(BM_AddValue20);
BENCHMARK(BM_AddValue50);
BENCHMARK(BM_GetPercentile20Lower);
BENCHMARK(BM_GetPercentile20Higher);
BENCHMARK(BM_GetPercentile20WholeRange);
BENCHMARK(BM_GetPercentile50Middle);
BENCHMARK(BM_GetAvg20);
BENCHMARK(BM_GetAvg50);

// Run the benchmark
BENCHMARK_MAIN();