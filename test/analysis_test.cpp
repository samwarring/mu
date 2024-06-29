#include "mu_test.hpp"

using mu::detail::analysis;
using mu::detail::analysis_object;

CONSTEXPR_TEST(MuAnalysis, EmptyToEmpty) {
  using from = mu::mult<>;
  using to = mu::mult<>;
  static_assert(analysis_object<from, to>.is_convertible);
  static_assert(analysis_object<from, to>.is_equivalent);
}

CONSTEXPR_TEST(MuAnalysis, ApplesToApples) {
  using from = test::apples;
  using to = test::apples;
  static_assert(analysis_object<from, to>.is_convertible);
  static_assert(analysis_object<from, to>.is_equivalent);
}

CONSTEXPR_TEST(MuAnalysis, ApplesToOranges) {
  using from = test::apples;
  using to = test::oranges;
  static_assert(!analysis_object<from, to>.is_convertible);
  static_assert(!analysis_object<from, to>.is_equivalent);
}

CONSTEXPR_TEST(MuAnalysis, ApplesPerApplesToRatio) {
  using from = mu::mult<test::apples, mu::pow<test::apples, -1>>;
  using to = std::ratio<1>;
  static_assert(analysis_object<from, to>.is_convertible);
  static_assert(analysis_object<from, to>.is_equivalent);
}

TEST(MuAnalysis, ApplesPerApplesToRatio) {
  using from = mu::mult<test::apples, mu::pow<test::apples, -1>>;
  using to = std::ratio<1>;
  analysis<from, to> a;
  ASSERT_TRUE(a.is_convertible);
  ASSERT_TRUE(a.is_equivalent);
  ASSERT_TRUE(a.is_int_convertible);
  ASSERT_EQ(a.int_conversion, 1);
}

CONSTEXPR_TEST(MuAnalysis, ApplesToHalfApples) {
  using from = test::apples;
  using to = mu::mult<std::ratio<1, 2>, test::apples>;
  static_assert(analysis_object<from, to>.is_convertible);
  static_assert(!analysis_object<from, to>.is_equivalent);
  static_assert(analysis_object<from, to>.is_int_convertible);
  static_assert(analysis_object<from, to>.int_conversion == 2);
}

CONSTEXPR_TEST(MuAnalysis, ApplesToDozenApples) {
  using from = test::apples;
  using to = mu::mult<std::ratio<12>, test::apples>;
  static_assert(analysis_object<from, to>.is_convertible);
  static_assert(!analysis_object<from, to>.is_equivalent);
  static_assert(!analysis_object<from, to>.is_int_convertible);
  static_assert(!analysis_object<from, to>.is_int_convertible);
  static_assert(
      is_equal(analysis_object<from, to>.float_conversion, 1.0 / 12.0));
}