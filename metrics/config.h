#pragma once

#include <metrics/counter.h>
#include <metrics/gauge.h>
#include <metrics/histogram.h>

#include <array>
#include <string>

namespace metrics {

// clang-format off

// --------------------Counters--------------------------
enum CounterName {
  CounterPlaceOrder,
  CounterCancelOrder,
  CounterTest,
  NumCounters
};

extern std::array<std::string, NumCounters> counter_keys;
extern std::array<Counter, NumCounters> counter_arr;

// --------------------Gauges--------------------------
enum GaugeName { GaugePnl, GaugeTest, NumGauges };

extern std::array<std::string, NumGauges> gauge_keys;
extern std::array<Gauge, NumGauges> gauge_arr;

// --------------------Histograms--------------------------
enum HistogramName {
  HistogramMsgLatency,
  HistogramTimeUsed,
  HistogramTest,
  NumHistograms
};

extern std::array<std::string, NumHistograms> histogram_keys;
extern std::array<Histogram, NumHistograms> histogram_arr;

// clang-format on

}  // namespace metrics