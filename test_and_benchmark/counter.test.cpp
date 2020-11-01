#include <gtest/gtest.h>
#include <metrics/counter.h>

using namespace metrics;

TEST(Counter, TestSimple) {
  Counter c;

  for (int i = 0; i < 10; ++i) {
    c.addValue(i);
  }

  EXPECT_EQ(45, c.getCounter());

  c.clear();

  EXPECT_EQ(0, c.getCounter());
}