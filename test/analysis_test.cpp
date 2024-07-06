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
  using from = apples;
  using to = apples;
  static_assert(analysis_object<from, to>.is_convertible);
  static_assert(analysis_object<from, to>.is_equivalent);
}

CONSTEXPR_TEST(MuAnalysis, ApplesToOranges) {
  using from = apples;
  using to = oranges;
  static_assert(!analysis_object<from, to>.is_convertible);
  static_assert(!analysis_object<from, to>.is_equivalent);
}

CONSTEXPR_TEST(MuAnalysis, ApplesPerApplesToRatio) {
  using from = mu::mult<apples, mu::pow<apples, -1>>;
  using to = std::ratio<1>;
  static_assert(analysis_object<from, to>.is_convertible);
  static_assert(analysis_object<from, to>.is_equivalent);
}

TEST(MuAnalysis, ApplesPerApplesToRatio) {
  using from = mu::mult<apples, mu::pow<apples, -1>>;
  using to = std::ratio<1>;
  analysis<from, to> a;
  ASSERT_TRUE(a.is_convertible);
  ASSERT_TRUE(a.is_equivalent);
  ASSERT_TRUE(a.is_int_convertible);
  ASSERT_EQ(a.int_conversion, 1);
}

CONSTEXPR_TEST(MuAnalysis, ApplesToHalfApples) {
  using from = apples;
  using to = mu::mult<std::ratio<1, 2>, apples>;
  static_assert(analysis_object<from, to>.is_convertible);
  static_assert(!analysis_object<from, to>.is_equivalent);
  static_assert(analysis_object<from, to>.is_int_convertible);
  static_assert(analysis_object<from, to>.int_conversion == 2);
}

CONSTEXPR_TEST(MuAnalysis, ApplesToDozenApples) {
  using from = apples;
  using to = mu::mult<std::ratio<12>, apples>;
  static_assert(analysis_object<from, to>.is_convertible);
  static_assert(!analysis_object<from, to>.is_equivalent);
  static_assert(!analysis_object<from, to>.is_int_convertible);
  static_assert(!analysis_object<from, to>.is_int_convertible);
  static_assert(
      is_equal(analysis_object<from, to>.float_conversion, 1.0 / 12.0));
}