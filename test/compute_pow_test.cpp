#include "mu_test.hpp"

using mu::detail::compute_rational_pow;
using mu::detail::compute_whole_pow_float;
using mu::detail::compute_whole_pow_int;
using mu::detail::is_infinity;
using mu::detail::safe_mult;

TEST(MuComputePow, IntPowersOf2) {
  ASSERT_EQ(*compute_whole_pow_int(2, 0), 1);
  ASSERT_EQ(*compute_whole_pow_int(2, 1), 2);
  ASSERT_EQ(*compute_whole_pow_int(2, 2), 4);
  ASSERT_EQ(*compute_whole_pow_int(2, 3), 8);
  ASSERT_EQ(*compute_whole_pow_int(2, 4), 16);
  ASSERT_EQ(*compute_whole_pow_int(2, 5), 32);
  ASSERT_EQ(*compute_whole_pow_int(2, 6), 64);
  ASSERT_EQ(*compute_whole_pow_int(2, 7), 128);
  ASSERT_EQ(*compute_whole_pow_int(2, 8), 256);
  ASSERT_EQ(*compute_whole_pow_int(2, 9), 512);
  ASSERT_EQ(*compute_whole_pow_int(2, 10), 1024);
  ASSERT_EQ(*compute_whole_pow_int(2, 62), 4611686018427387904ll);
  ASSERT_EQ(compute_whole_pow_int(2, 256), std::nullopt);
}

TEST(MuComputePow, FloatPowersOf2) {
  ASSERT_EQ(compute_whole_pow_float(2, 0), 1);
  ASSERT_EQ(compute_whole_pow_float(2, 1), 2);
  ASSERT_EQ(compute_whole_pow_float(2, 2), 4);
  ASSERT_EQ(compute_whole_pow_float(2, 3), 8);
  ASSERT_EQ(compute_whole_pow_float(2, 4), 16);
  ASSERT_EQ(compute_whole_pow_float(2, 5), 32);
  ASSERT_EQ(compute_whole_pow_float(2, 6), 64);
  ASSERT_EQ(compute_whole_pow_float(2, 7), 128);
  ASSERT_EQ(compute_whole_pow_float(2, 8), 256);
  ASSERT_EQ(compute_whole_pow_float(2, 9), 512);
  ASSERT_EQ(compute_whole_pow_float(2, 10), 1024);
  ASSERT_EQ(compute_whole_pow_float(2, 62), 4611686018427387904ll);
  ASSERT_EQ(compute_whole_pow_float(2, 100),
            1.267650600228229401496703205376e30);
  ASSERT_TRUE(is_infinity(compute_whole_pow_float(2, 1'000'000)));
}

TEST(MuComputePow, RationalPowersOf2) {
  ASSERT_LONG_DOUBLE_EQ(*compute_rational_pow(2.0, {1, 2}),
                        1.4142135623730950488);
  ASSERT_LONG_DOUBLE_EQ(*compute_rational_pow(2.0, {3, 2}),
                        2.8284271247461900976);
  ASSERT_LONG_DOUBLE_EQ(*compute_rational_pow(2.0, {50, 26}),
                        3.7923100573566858585);
  ASSERT_LONG_DOUBLE_EQ(*compute_rational_pow(2.0, {41, 2}),
                        1.4829104003789305139e6);
}

TEST(MuComputePow, DivideBy0) { ASSERT_FALSE(compute_rational_pow(0.0, -1)); }

TEST(MuComputePow, EvenRootOfNegative) {
  ASSERT_FALSE(compute_rational_pow(-4.0, {1, 2}));
}

TEST(MuComputePow, OddRootOfNegative) {
  auto result = compute_rational_pow(-8.0, {1, 3});
  ASSERT_TRUE(result);
  ASSERT_LONG_DOUBLE_EQ(*result, -2.0);
}

TEST(MuComputePow, SafeMult) {
  constexpr static auto MAX = std::numeric_limits<std::intmax_t>::max();
  constexpr static auto MIN = std::numeric_limits<std::intmax_t>::min();

  ASSERT_EQ(*safe_mult(4, 5), 20);
  ASSERT_EQ(*safe_mult(-4, 5), -20);
  ASSERT_EQ(*safe_mult(MAX, 1), MAX);
  ASSERT_EQ(*safe_mult(MIN, 1), MIN);
  ASSERT_EQ(safe_mult(MAX, -1), MIN + 1);
  ASSERT_FALSE(safe_mult(MIN, -1));
  ASSERT_FALSE(safe_mult(MAX, 2));
  ASSERT_FALSE(safe_mult(MAX, -2));
  ASSERT_FALSE(safe_mult(MIN, 2));
  ASSERT_FALSE(safe_mult(MIN, -2));
  ASSERT_FALSE(safe_mult(MIN + 1, MIN + 1));
}