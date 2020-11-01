#include <metrics/config.h>

#include <string>

#include "rapidjson/writer.h"  // for stringify JSON

namespace metrics {

inline Counter& GetCounter(CounterName counter) { return counter_arr[counter]; }
inline Gauge& GetGauge(GaugeName gauge) { return gauge_arr[gauge]; }
inline Histogram& GetHisogram(HistogramName histogram) {
  return histogram_arr[histogram];
}

std::string GetJson() {
  using namespace rapidjson;

  StringBuffer sb;
  Writer<StringBuffer> writer(sb);

  writer.StartObject();

  // tags
  writer.String("hostname");
  writer.String("hb14");

  writer.String("ServiceName");
  writer.String("magic");

  writer.String("CoreId");
  writer.Int(2);

  // statistics
  for (int i = 0; i < NumCounters; ++i) {
    writer.String(counter_keys[i].c_str());
    writer.Int64(counter_arr[i].getCounter());
  }

  for (int i = 0; i < NumGauges; ++i) {
    writer.String(gauge_keys[i].c_str());
    writer.Double(gauge_arr[i].getGauge());
  }

  for (int i = 0; i < NumHistograms; ++i) {
    writer.String((histogram_keys[i] + "Avg").c_str());
    writer.Int64(histogram_arr[i].getAvg());

    writer.String((histogram_keys[i] + "95").c_str());
    writer.Int64(histogram_arr[i].getPercentileEstimate(0.95));

    writer.String((histogram_keys[i] + "99").c_str());
    writer.Int64(histogram_arr[i].getPercentileEstimate(0.99));
  }

  writer.EndObject();

  return sb.GetString();
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

std::string SerializeAndRefresh() {
  std::string tmp = GetJson();
  Refresh();
  return tmp;
}

}  // namespace metrics