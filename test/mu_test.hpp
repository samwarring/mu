#ifndef INCLUDED_MU_TEST_HPP
#define INCLUDED_MU_TEST_HPP
#include <gtest/gtest.h>
#include <limits>
#include <mu/mu.hpp>

#define ASSERT_LONG_DOUBLE_EQ(actual, expected)                                \
  ASSERT_DOUBLE_EQ((double)(actual), (double)(expected))

#define CONSTEXPR_TEST(suite, case) namespace test_##suite##_##case

constexpr bool is_equal(long double a, long double b) {
  long double delta = a - b;
  delta = delta < 0 ? -delta : delta;
  long double tolerance = std::numeric_limits<long double>::max() * 2;
  return delta < tolerance;
}

struct apples {
  constexpr static const char *name = "apples";
  constexpr static const char *symbol = "🍎";
};

struct oranges {
  constexpr static const char *name = "oranges";
  constexpr static const char *symbol = "🍊";
};

struct golden {
  constexpr static const char *name = "golden";
  constexpr static const char *symbol = "φ";
  constexpr static long double value = 1.618033988749;
};

struct golden_apples {
  constexpr static const char *name = "golden_apples";
  constexpr static const char *symbol = "🍏";
  using units = mu::mult<golden, apples>;
};

struct universal_fruit_constant {
  constexpr static const char *name = "universal_fruit_constant";
  constexpr static const char *symbol = "Ω";
  constexpr static long double value = 7.499135;
  using units = mu::mult<apples, oranges>;
};

#endif