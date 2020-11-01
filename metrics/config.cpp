#include <metrics/config.h>
#include <metrics/counter.h>
#include <metrics/gauge.h>
#include <metrics/histogram.h>

#include <array>
#include <string>

namespace metrics {

// clang-format off

// --------------------Counters--------------------------
std::array<std::string, NumCounters> counter_keys = {
    "CounterPlaceOrder",
    "CounterCancelOrder",
    "CounterTest"
};
std::array<Counter, NumCounters> counter_arr;

// --------------------Gauges--------------------------
std::array<std::string, NumGauges> gauge_keys = {
    "GaugePnl",
    "GaugeTest"
};
std::array<Gauge, NumGauges> gauge_arr;

// --------------------Histograms--------------------------
std::array<std::string, NumHistograms> histogram_keys = {
    "HistogramMsgLatency",
    "HistogramTimeUsed",
    "HistogramTest"
};
std::array<Histogram, NumHistograms> histogram_arr = {
    Histogram{100, 0, 1000},
    Histogram{1000, 0, 20'000},
    Histogram{1, 0, 1000}
};

// clang-format on

}  // namespace metrics