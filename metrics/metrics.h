#include <metrics/config.h>

#include <ostream>
#include <string>

namespace metrics {

inline Counter& GetCounter(CounterName counter) { return counter_arr[counter]; }
inline Gauge& GetGauge(GaugeName gauge) { return gauge_arr[gauge]; }
inline Histogram& GetHisogram(HistogramName histogram) { return histogram_arr[histogram]; }

struct MetricsData {
  std::string host_name{};
  std::string service_name{};
  int core_id{};

  std::array<std::int64_t, NumCounters> counters;
  std::array<double, NumGauges> gauges;
  std::array<std::int64_t, NumHistograms> avgs;
  std::array<std::int64_t, NumHistograms> v95s;
  std::array<std::int64_t, NumHistograms> v99s;

  friend std::ostream& operator<<(std::ostream& os, const MetricsData& data);
};

inline std::ostream& operator<<(std::ostream& os, const MetricsData& data) {
  os << "host_name: " << data.host_name << " service_name: " << data.service_name << " core_id: " << data.core_id
     << " counters: [";
  for (int i = 0; i < NumCounters; ++i) os << data.counters[i] << " ";

  os << "] gauges: [";
  for (int i = 0; i < NumGauges; ++i) os << data.gauges[i] << " ";

  os << "] avgs: [";
  for (int i = 0; i < NumHistograms; ++i) os << data.avgs[i] << " ";

  os << "] v95s: [";
  for (int i = 0; i < NumHistograms; ++i) os << data.v95s[i] << " ";

  os << "] v99s: [";
  for (int i = 0; i < NumHistograms; ++i) os << data.v99s[i] << " ";
  os << "]";

  return os;
}

MetricsData GetMetricsData() {
  MetricsData metrics_data;

  // statistics
  for (int i = 0; i < NumCounters; ++i) {
    metrics_data.counters[i] = counter_arr[i].getCounter();
  }

  for (int i = 0; i < NumGauges; ++i) {
    metrics_data.gauges[i] = gauge_arr[i].getGauge();
  }

  for (int i = 0; i < NumHistograms; ++i) {
    metrics_data.avgs[i] = histogram_arr[i].getAvg();
    metrics_data.v95s[i] = histogram_arr[i].getPercentileEstimate(0.95);
    metrics_data.v99s[i] = histogram_arr[i].getPercentileEstimate(0.99);
  }

  return metrics_data;
}

void Refresh() {
  for (int i = 0; i < NumCounters; ++i) {
    counter_arr[i].clear();
  }

  for (int i = 0; i < NumGauges; ++i) {
    gauge_arr[i].clear();
  }

  for (int i = 0; i < NumHistograms; ++i) {
    histogram_arr[i].clear();
  }
}

MetricsData GetMetricsDataAndRefresh() {
  MetricsData metrics_data = GetMetricsData();
  Refresh();
  return metrics_data;
}

}  // namespace metrics