#include "mu_test.hpp"
#include <iostream>

using mu::detail::prime_factor;
using mu::detail::ratio;

constexpr std::vector<prime_factor> prime_factorize(ratio value,
                                                    ratio exponent = 1) {
  std::vector<prime_factor> out;
  mu::detail::prime_factorize(out, value, exponent);
  return out;
}

constexpr bool prime_factors_equal(ratio value, ratio exponent,
                                   std::vector<prime_factor> expected) {
  std::vector<prime_factor> actual;
  mu::detail::prime_factorize(actual, value, exponent);
  return actual == expected;
}

namespace mu::detail {
std::ostream &operator<<(std::ostream &out, const prime_factor &f) {
  out << f.base << '^' << f.exponent.num;
  if (f.exponent.den != 1) {
    out << '/' << f.exponent.den;
  }
  return out;
}
} // namespace mu::detail

TEST(MuPrimes, FactorsOf1000) {
  constexpr ratio value = 1000;
  constexpr ratio exponent = 1;
  std::vector<prime_factor> expected = {{2, 3}, {5, 3}};
  std::vector<prime_factor> actual = prime_factorize(value, exponent);
  ASSERT_EQ(actual, expected);
  static_assert(prime_factors_equal(value, exponent, {{2, 3}, {5, 3}}));
}

TEST(MuPrimes, FactorsOf123456) {
  ratio value = 123456;
  ratio exponent = 1;
  std::vector<prime_factor> expected = {{2, 6}, {3, 1}, {643, 1}};
  std::vector<prime_factor> actual = prime_factorize(value, exponent);
  ASSERT_EQ(actual, expected);
}

TEST(MuPrimes, FactorsOf1000Over123456) {
  ratio value{1000, 123456};
  ratio exponent = 1;
  std::vector<prime_factor> expected = {
      {2, 3}, {5, 3}, {2, -6}, {3, -1}, {643, -1}};
  std::vector<prime_factor> actual = prime_factorize(value, exponent);
  ASSERT_EQ(actual, expected);
}

TEST(MuPrimes, FactorsOf123456ToThe1Over2) {
  ratio value = 123456;
  ratio exponent{1, 2};
  std::vector<prime_factor> expected = {
      {2, {6, 2}}, {3, {1, 2}}, {643, {1, 2}}};
  std::vector<prime_factor> actual = prime_factorize(value, exponent);
  ASSERT_EQ(actual, expected);
}

TEST(MuPrimes, FactorsOf10Over7ToThe2Over3) {
  ratio value{10, 7};
  ratio exponent{2, 3};
  std::vector<prime_factor> expected = {{2, {2, 3}}, {5, {2, 3}}, {7, {-2, 3}}};
  std::vector<prime_factor> actual = prime_factorize(value, exponent);
  ASSERT_EQ(actual, expected);
}