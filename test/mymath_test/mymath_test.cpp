#include <gtest/gtest.h>
#include <mymath.hpp>

TEST(MathTest, QuadraticRealRoots) {
  double a = 1, b = 0, c = -1;
  auto [r1, r2] = mymath::quadratic(a, b, c);
  auto lesser = r1.real() < r2.real() ? r1 : r2;
  auto greater = r1.real() < r2.real() ? r2 : r1;
  ASSERT_TRUE(lesser == -1.0);
  ASSERT_TRUE(greater == 1.0);
}

TEST(MathTest, QuadriaticRepeatedRoot) {
  double a = 1, b = 0, c = 0;
  auto [r1, r2] = mymath::quadratic(a, b, c);
  ASSERT_TRUE(r1 == 0.0);
  ASSERT_TRUE(r2 == 0.0);
}

TEST(MathTest, QuadraticImaginaryRoots) {
  double a = 1, b = 0, c = 1;
  auto [r1, r2] = mymath::quadratic(a, b, c);
  ASSERT_TRUE(r1.imag() != 0.0);
  ASSERT_TRUE(r2.imag() != 0.0);
}