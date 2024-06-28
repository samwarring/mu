#include "mu_test.hpp"

using mu::detail::analysis;
using mu::detail::analyzed;

CONSTEXPR_TEST(MuAnalysis, EmptyToEmpty) {
  using from = mu::mult<>;
  using to = mu::mult<>;
  static_assert(analyzed<from, to>.is_convertible);
  static_assert(analyzed<from, to>.is_equivalent);
}

CONSTEXPR_TEST(MuAnalysis, ApplesToApples) {
  using from = test::apples;
  using to = test::apples;
  static_assert(analyzed<from, to>.is_convertible);
  static_assert(analyzed<from, to>.is_equivalent);
}

CONSTEXPR_TEST(MuAnalysis, ApplesToOranges) {
  using from = test::apples;
  using to = test::oranges;
  static_assert(!analyzed<from, to>.is_convertible);
  static_assert(!analyzed<from, to>.is_equivalent);
}

CONSTEXPR_TEST(MuAnalysis, ApplesPerApplesToRatio) {
  using from = mu::mult<test::apples, mu::pow<test::apples, -1>>;
  using to = std::ratio<2>;
  static_assert(analyzed<from, to>.is_convertible);
  static_assert(analyzed<from, to>.is_equivalent);
}

CONSTEXPR_TEST(MuAnalysis, ApplesToHalfApples) {
  using from = test::apples;
  using to = mu::mult<std::ratio<1, 2>, test::apples>;
  static_assert(analyzed<from, to>.is_convertible);
  static_assert(!analyzed<from, to>.is_equivalent);
  static_assert(analyzed<from, to>.is_int_convertible);
  static_assert(analyzed<from, to>.int_conversion == 2);
}