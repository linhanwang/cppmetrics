#include <metrics/config.h>

namespace metrics {

inline Counter& GetCounter(CounterName counter) { return counter_arr[counter]; }
inline Gauge& GetGauge(GaugeName gauge) { return gauge_arr[gauge]; }
inline Histogram& GetHisogram(HistogramName histogram) {
  return histogram_arr[histogram];
}

}  // namespace metrics