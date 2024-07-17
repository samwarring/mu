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

TEST(MuAnalysis, ApplesToDozenApples) {
  using from = apples;
  using to = mu::mult<std::ratio<12>, apples>;
  analysis<from, to> a;
  ASSERT_TRUE(a.is_convertible);
  ASSERT_TRUE(!a.is_equivalent);
  ASSERT_TRUE(!a.is_int_convertible);
  ASSERT_EQ((double)a.float_conversion, 1.0 / 12.0);
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
      is_equal(analysis_object<from, to>.float_conversion, 1.0l / 12.0l));
}

TEST(MuAnalysis, NegativeIntegerScale) {
  using n8_oranges = mu::mult<std::ratio<-8>, oranges>;
  using sqrt_oranges = mu::pow<oranges, 1, 2>;
  using sqrt_n8_oranges = mu::pow<n8_oranges, 1, 2>;
  using cbrt_oranges = mu::pow<oranges, 1, 3>;
  using cbrt_n8_oranges = mu::pow<n8_oranges, 1, 3>;
  {
    analysis<n8_oranges, oranges> a;
    ASSERT_TRUE(a.is_convertible);
    ASSERT_FALSE(a.is_equivalent);
    ASSERT_TRUE(a.is_int_convertible);
    ASSERT_EQ(a.int_conversion, -8);
  }
  {
    analysis<oranges, n8_oranges> a;
    ASSERT_TRUE(a.is_convertible);
    ASSERT_FALSE(a.is_equivalent);
    ASSERT_FALSE(a.is_int_convertible);
    ASSERT_EQ(a.float_conversion, (long double)(-1) / 8);
  }
  {
    // sqrt of negative number. Not convertible.
    analysis<sqrt_oranges, sqrt_n8_oranges> a;
    ASSERT_FALSE(a.is_convertible);
    ASSERT_FALSE(a.is_equivalent);
  }
  {
    // sqrt of negative number. Not convertible.
    analysis<sqrt_n8_oranges, sqrt_oranges> a;
    ASSERT_FALSE(a.is_convertible);
    ASSERT_FALSE(a.is_equivalent);
  }
  {
    // cube root of -8 is -2. This is int convertible.
    analysis<cbrt_n8_oranges, cbrt_oranges> a;
    ASSERT_TRUE(a.is_convertible);
    ASSERT_FALSE(a.is_equivalent);
    ASSERT_TRUE(a.is_int_convertible);
    ASSERT_EQ(a.int_conversion, -2);
  }
  {
    // cube root of -8 is -2. This is int convertible.
    analysis<cbrt_oranges, cbrt_n8_oranges> a;
    ASSERT_TRUE(a.is_convertible);
    ASSERT_FALSE(a.is_equivalent);
    ASSERT_FALSE(a.is_int_convertible);
    ASSERT_EQ(a.float_conversion, (long double)-0.5);
  }
}
