#include "mu_test.hpp"

using mu::units_conversion_t;
using mu::units_conversion_v;
using mu::units_convertible_to;
using mu::units_equivalent_to;

CONSTEXPR_TEST(MuConversion, ApplesToOranges) {
  using from = apples;
  using to = oranges;
  static_assert(!units_convertible_to<from, to>);
  static_assert(!units_equivalent_to<from, to>);
}

CONSTEXPR_TEST(MuConversion, ApplesToApples) {
  using from = apples;
  using to = apples;
  constexpr int expected_conversion = 1;
  static_assert(units_convertible_to<from, to>);
  static_assert(units_equivalent_to<from, to>);
  static_assert(units_conversion_v<from, to> == expected_conversion);
}

CONSTEXPR_TEST(MuConversion, EmptyToEmpty) {
  using from = mu::mult<>;
  using to = mu::mult<>;
  constexpr int expected_conversion = 1;
  static_assert(units_convertible_to<from, to>);
  static_assert(units_equivalent_to<from, to>);
  static_assert(units_conversion_v<from, to> == expected_conversion);
}

CONSTEXPR_TEST(MuConversion, EmptyToHalf) {
  using from = mu::mult<>;
  using to = std::ratio<1, 2>;
  constexpr int expected_conversion = 2;
  static_assert(units_convertible_to<from, to>);
  static_assert(!units_equivalent_to<from, to>);
  static_assert(units_conversion_v<from, to> == expected_conversion);
}

namespace /* local to this file only */ {
struct test_feet {
  constexpr static const char *name = "feet";
  constexpr static const char *symbol = "ft";
};
struct test_meters {
  constexpr static const char *name = "meters";
  constexpr static const char *symbol = "m";
  constexpr static long double value = 3.280839895; // feet
  using units = test_feet;
};
} // namespace

TEST(MuConversion, SqFeetToSqMeters) {
  using from = mu::pow<test_feet, 2>;
  using to = mu::pow<test_meters, 2>;
  mu::detail::analysis<from, to> a;
  constexpr long double expected_conversion =
      0.0929030400007432243200044593459200237831782401189158912005707962;
  ASSERT_TRUE((units_convertible_to<from, to>));
  ASSERT_FALSE((units_equivalent_to<from, to>));
  ASSERT_TRUE((is_equal(units_conversion_v<from, to>, expected_conversion)));
}

CONSTEXPR_TEST(MuConversion, SqFeetToSqMeters) {
  using from = mu::pow<test_feet, 2>;
  using to = mu::pow<test_meters, 2>;
  constexpr long double expected_conversion =
      0.0929030400007432243200044593459200237831782401189158912005707962;
  static_assert(units_convertible_to<from, to>);
  static_assert(!units_equivalent_to<from, to>);
  static_assert(is_equal(units_conversion_v<from, to>, expected_conversion));
}

CONSTEXPR_TEST(MuConversion, NegativeScale) {
  // Can be scaled by negative value.
  using funky_apples = mu::mult<funky, apples>; // -1.234 apples
  static_assert(units_convertible_to<apples, funky_apples>);

  // Cannot be converted, requires even root of negative value.
  using sqrt_apples = mu::pow<apples, 1, 2>;
  using sqrt_funky_apples = mu::pow<funky_apples, 1, 2>;
  static_assert(!units_convertible_to<sqrt_apples, sqrt_funky_apples>);

  // Can be converted, requires odd root of negative value, which is ok.
  using cbrt_apples = mu::pow<apples, 1, 3>;
  using cbrt_funky_apples = mu::pow<funky_apples, 1, 3>;
  static_assert(units_convertible_to<cbrt_apples, cbrt_funky_apples>);
  static_assert(units_conversion_v<cbrt_apples, cbrt_funky_apples> < 0);
}
