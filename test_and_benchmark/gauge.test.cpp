#include <gtest/gtest.h>
#include <metrics/gauge.h>

using namespace metrics;

TEST(Counter, TestSimple) {
  Gauge g;

  for (int i = 0; i < 10; ++i) {
    g.addValue(i);
    EXPECT_EQ(i, g.getGauge());
    g.clear();
    EXPECT_EQ(i, g.getGauge());
  }
}