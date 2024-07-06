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

CONSTEXPR_TEST(MuConversion, SqFeetToSqMeters) {
  struct test_feet {
    constexpr static const char *name = "feet";
    constexpr static const char *symbol = "ft";
  };
  struct test_meters {
    constexpr static const char *name = "meters";
    constexpr static const char *symbol = "m";
    constexpr static long double value = 3.28084;
    using units = test_feet;
  };
  using from = mu::pow<test_feet, 2>;
  using to = mu::pow<test_meters, 2>;
  constexpr long double expected_conversion = 0.092903;
  static_assert(units_convertible_to<from, to>);
  static_assert(!units_equivalent_to<from, to>);
  static_assert(is_equal(units_conversion_v<from, to>, expected_conversion));
}