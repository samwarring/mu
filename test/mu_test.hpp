#ifndef INCLUDED_MU_TEST_HPP
#define INCLUDED_MU_TEST_HPP
#include <gtest/gtest.h>
#include <mu/mu.hpp>

#define CONSTEXPR_TEST(suite, case) namespace test_##suite##_##case

namespace test {

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

} // namespace test

#endif