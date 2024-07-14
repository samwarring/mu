#include "mu_test.hpp"

using mu::detail::compute_rational_pow;
using mu::detail::compute_whole_pow;

TEST(MuComputePow, PowersOf2) {
  ASSERT_EQ(compute_whole_pow(2, 0), 1);
  ASSERT_EQ(compute_whole_pow(2, 1), 2);
  ASSERT_EQ(compute_whole_pow(2, 2), 4);
  ASSERT_EQ(compute_whole_pow(2, 3), 8);
  ASSERT_EQ(compute_whole_pow(2, 4), 16);
  ASSERT_EQ(compute_whole_pow(2, 5), 32);
  ASSERT_EQ(compute_whole_pow(2, 6), 64);
  ASSERT_EQ(compute_whole_pow(2, 7), 128);
  ASSERT_EQ(compute_whole_pow(2, 8), 256);
  ASSERT_EQ(compute_whole_pow(2, 9), 512);
  ASSERT_EQ(compute_whole_pow(2, 10), 1024);
}

TEST(MuComputePow, RationalPowersOf2) {
  ASSERT_LONG_DOUBLE_EQ((compute_rational_pow(2.0, {1, 2})).second,
                        1.4142135623730950488);
  ASSERT_LONG_DOUBLE_EQ((compute_rational_pow(2.0, {3, 2})).second,
                        2.8284271247461900976);
  ASSERT_LONG_DOUBLE_EQ((compute_rational_pow(2.0, {50, 26})).second,
                        3.7923100573566858585);
  ASSERT_LONG_DOUBLE_EQ((compute_rational_pow(2.0, {41, 2})).second,
                        1.4829104003789305139e6);
}

TEST(MuComputePow, DivideBy0) {
  auto [valid, result] = compute_rational_pow(0.0, -1);
  ASSERT_FALSE(valid);
}

TEST(MuComputePow, EvenRootOfNegative) {
  auto [valid, result] = compute_rational_pow(-4.0, {1, 2});
  ASSERT_FALSE(valid);
}

TEST(MuComputePow, OddRootOfNegative) {
  auto [valid, result] = compute_rational_pow(-8.0, {1, 3});
  ASSERT_TRUE(valid);
  ASSERT_LONG_DOUBLE_EQ(result, -2.0);
}