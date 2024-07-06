#include "mu_test.hpp"

using mu::detail::base_constant;
using mu::detail::base_unit;
using mu::detail::composite_constant_value;
using mu::detail::factor;
using mu::detail::factor_traits;
using mu::detail::ratio;

template <class Factor, class ExpectedBase>
constexpr bool has_base =
    std::is_same_v<typename factor_traits<Factor>::base, ExpectedBase>;

template <class Factor>
constexpr bool has_exponent(const ratio &expected_exponent) {
  return factor_traits<Factor>::exponent == expected_exponent;
}

template <class Factor>
constexpr bool is_dimensional = factor_traits<Factor>::is_dimensional;

template <class Factor>
constexpr bool has_rational_value(const ratio &expected_value) {
  return factor_traits<Factor>::is_rational_value &&
         factor_traits<Factor>::rational_value == expected_value;
}

template <class Factor>
constexpr bool has_irrational_value(long double expected_value) {
  return !factor_traits<Factor>::is_rational_value &&
         factor_traits<Factor>::irrational_value == expected_value;
}

CONSTEXPR_TEST(MuFactor, StdRatio) {
  using t = std::ratio<2, 3>;
  using expected_base = std::ratio<2, 3>;
  constexpr static ratio expected_exponent = 1;
  constexpr static ratio expected_value{2, 3};
  static_assert(factor<t>);
  static_assert(has_base<t, expected_base>);
  static_assert(!is_dimensional<t>);
  static_assert(has_exponent<t>(expected_exponent));
  static_assert(has_rational_value<t>(expected_value));
}

CONSTEXPR_TEST(MuFactor, BaseUnit) {
  using t = apples;
  using expected_base = apples;
  constexpr static ratio expected_exponent = 1;
  constexpr static ratio expected_value = 1;
  static_assert(base_unit<t>);
  static_assert(factor<t>);
  static_assert(has_base<t, expected_base>);
  static_assert(is_dimensional<t>);
  static_assert(has_exponent<t>(expected_exponent));
  static_assert(has_rational_value<t>(expected_value));
}

CONSTEXPR_TEST(MuFactor, BaseConstant) {
  using t = golden;
  using expected_base = golden;
  constexpr static ratio expected_exponent = 1;
  constexpr static long double expected_value = golden::value;
  static_assert(base_constant<t>);
  static_assert(factor<t>);
  static_assert(has_base<t, expected_base>);
  static_assert(!is_dimensional<t>);
  static_assert(has_exponent<t>(expected_exponent));
  static_assert(has_irrational_value<t>(expected_value));
}

CONSTEXPR_TEST(MuFactor, CompositeConstantValue) {
  using t = composite_constant_value<universal_fruit_constant>;
  using expected_base = composite_constant_value<universal_fruit_constant>;
  constexpr static ratio expected_exponent = 1;
  constexpr static long double expected_value = universal_fruit_constant::value;
  static_assert(factor<t>);
  static_assert(has_base<t, expected_base>);
  static_assert(!is_dimensional<t>);
  static_assert(has_exponent<t>(expected_exponent));
  static_assert(has_irrational_value<t>(expected_value));
}

CONSTEXPR_TEST(MuFactor, BaseUnitSquared) {
  using t = mu::pow<oranges, 2>;
  using expected_base = oranges;
  constexpr static ratio expected_exponent = 2;
  constexpr static ratio expected_value = 1;
  static_assert(factor<t>);
  static_assert(has_base<t, expected_base>);
  static_assert(is_dimensional<t>);
  static_assert(has_exponent<t>(expected_exponent));
  static_assert(has_rational_value<t>(expected_value));
}

CONSTEXPR_TEST(MuFactor, BaseConstantSquareRoot) {
  using t = mu::pow<golden, 1, 2>;
  using expected_base = golden;
  constexpr static ratio expected_exponent{1, 2};
  constexpr static long double expected_value = golden::value;
  static_assert(factor<t>);
  static_assert(has_base<t, expected_base>);
  static_assert(!is_dimensional<t>);
  static_assert(has_exponent<t>(expected_exponent));
  static_assert(has_irrational_value<t>(expected_value));
}

CONSTEXPR_TEST(MuFactor, BaseUnitSquaredSquared) {
  using t = mu::pow<mu::pow<oranges, 2>, 2>;
  using expected_base = oranges;
  constexpr static ratio expected_exponent = 4;
  constexpr static ratio expected_value = 1;
  static_assert(factor<t>);
  static_assert(has_base<t, expected_base>);
  static_assert(is_dimensional<t>);
  static_assert(has_exponent<t>(expected_exponent));
  static_assert(has_rational_value<t>(expected_value));
}