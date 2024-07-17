#include "mu_test.hpp"

using mu::units;
using mu::detail::composite_constant_value;
using mu::detail::unit_traits;

template <units T, class ExpectedFactors>
constexpr static bool has_factors =
    std::is_same_v<typename unit_traits<T>::factors, ExpectedFactors>;

template <units T>
constexpr bool has_format_names(std::string expected_format) {
  mu::format_options fopts;
  fopts.labels = mu::format_options::label_type::names;
  fopts.mult_sep = " * ";
  std::string formatted = mu::to_string<T>(fopts);
  return formatted == expected_format;
}

template <units T>
constexpr bool has_format_symbols(std::string expected_format) {
  mu::format_options fopts;
  fopts.labels = mu::format_options::label_type::symbols;
  fopts.mult_sep = " * ";
  std::string formatted = mu::to_string<T>(fopts);
  return formatted == expected_format;
}

CONSTEXPR_TEST(MuUnits, StdRatio) {
  using t = std::ratio<2, 3>;
  using expected_factors = mu::mult<std::ratio<2, 3>>;
  constexpr static const char *expected_format_names = "2/3";
  constexpr static const char *expected_format_symbols = "2/3";
  static_assert(units<t>);
  static_assert(has_factors<t, expected_factors>);
  static_assert(has_format_names<t>(expected_format_names));
  static_assert(has_format_symbols<t>(expected_format_symbols));
}

CONSTEXPR_TEST(MuUnits, BaseUnit) {
  using t = apples;
  using expected_factors = mu::mult<apples>;
  constexpr static const char *expected_format_names = "apples";
  constexpr static const char *expected_format_symbols = "🍎";
  static_assert(units<t>);
  static_assert(has_factors<t, expected_factors>);
  static_assert(has_format_names<t>(expected_format_names));
  static_assert(has_format_symbols<t>(expected_format_symbols));
}

CONSTEXPR_TEST(MuUnits, BaseConstant) {
  using t = golden;
  using expected_factors = mu::mult<golden>;
  constexpr static const char *expected_format_names = "golden";
  constexpr static const char *expected_format_symbols = "φ";
  static_assert(units<t>);
  static_assert(has_factors<t, expected_factors>);
  static_assert(has_format_names<t>(expected_format_names));
  static_assert(has_format_symbols<t>(expected_format_symbols));
}

CONSTEXPR_TEST(MuUnits, CompositeUnit) {
  using t = golden_apples;
  using expected_factors = mu::mult<golden, apples>;
  constexpr static const char *expected_format_names = "golden_apples";
  constexpr static const char *expected_format_symbols = "🍏";
  static_assert(units<t>);
  static_assert(has_factors<t, expected_factors>);
  static_assert(has_format_names<t>(expected_format_names));
  static_assert(has_format_symbols<t>(expected_format_symbols));
}

CONSTEXPR_TEST(MuUnits, CompositeConstant) {
  using t = universal_fruit_constant;
  using expected_factors =
      mu::mult<composite_constant_value<universal_fruit_constant>, apples,
               oranges>;
  constexpr static const char *expected_format_names =
      "universal_fruit_constant";
  constexpr static const char *expected_format_symbols = "Ω";
  static_assert(units<t>);
  static_assert(has_factors<t, expected_factors>);
  static_assert(has_format_names<t>(expected_format_names));
  static_assert(has_format_symbols<t>(expected_format_symbols));
}

CONSTEXPR_TEST(MuUnits, BaseUnitSquared) {
  using t = mu::pow<apples, 2>;
  using expected_factors = mu::mult<mu::pow<apples, 2>>;
  constexpr static const char *expected_format_names = "apples^2";
  constexpr static const char *expected_format_symbols = "🍎^2";
  static_assert(units<t>);
  static_assert(has_factors<t, expected_factors>);
  static_assert(has_format_names<t>(expected_format_names));
  static_assert(has_format_symbols<t>(expected_format_symbols));
}

CONSTEXPR_TEST(MuUnits, CompositeConstantSquared) {
  using t = mu::pow<universal_fruit_constant, 2>;
  using expected_factors =
      mu::mult<mu::pow<composite_constant_value<universal_fruit_constant>, 2>,
               mu::pow<apples, 2>, mu::pow<oranges, 2>>;
  constexpr static const char *expected_format_names =
      "universal_fruit_constant^2";
  constexpr static const char *expected_format_symbols = "Ω^2";
  static_assert(units<t>);
  static_assert(has_factors<t, expected_factors>);
  static_assert(has_format_names<t>(expected_format_names));
  static_assert(has_format_symbols<t>(expected_format_symbols));
}

CONSTEXPR_TEST(MuUnits, MultUnits) {
  using t = mu::mult<golden, oranges>;
  using expected_factors = mu::mult<golden, oranges>;
  constexpr static const char *expected_format_names = "golden * oranges";
  constexpr static const char *expected_format_symbols = "φ * 🍊";
  static_assert(units<t>);
  static_assert(has_factors<t, expected_factors>);
  static_assert(has_format_names<t>(expected_format_names));
  static_assert(has_format_symbols<t>(expected_format_symbols));
}

CONSTEXPR_TEST(MuUnits, MultUnitsSquared) {
  using t = mu::pow<mu::mult<golden, oranges>, 2>;
  using expected_factors = mu::mult<mu::pow<golden, 2>, mu::pow<oranges, 2>>;
  constexpr static const char *expected_format_names = "(golden * oranges)^2";
  constexpr static const char *expected_format_symbols = "(φ * 🍊)^2";
  static_assert(units<t>);
  static_assert(has_factors<t, expected_factors>);
  static_assert(has_format_names<t>(expected_format_names));
  static_assert(has_format_symbols<t>(expected_format_symbols));
}

CONSTEXPR_TEST(MuUnits, ContainsPrefix) {
  using t = mu::pow<mu::mult<basket, golden_apples>, 2>;
  using expected_factors = mu::mult<mu::pow<std::ratio<15>, 2>,
                                    mu::pow<golden, 2>, mu::pow<apples, 2>>;
  constexpr static const char *expected_format_names =
      "basket_of_golden_apples^2";
  constexpr static const char *expected_format_symbols = "🧺🍏^2";
  static_assert(units<t>);
  static_assert(has_factors<t, expected_factors>);
  static_assert(has_format_names<t>(expected_format_names));
  static_assert(has_format_symbols<t>(expected_format_symbols));
}

CONSTEXPR_TEST(MuUnits, ZeroScales) {
  struct positive_fp_zero {
    constexpr static const char *name = "+0.0";
    constexpr static long double value = +0.0;
  };
  struct negative_fp_zero {
    constexpr static const char *name = "-0.0";
    constexpr static long double value = -0.0;
  };
  using rzero = std::ratio<0>;
  using positive_fp_zero_apples = mu::mult<positive_fp_zero, apples>;
  using negative_fp_zero_apples = mu::mult<negative_fp_zero, apples>;

  // While these types are _not_ units, it is still valid code to test them for
  // satisfying `units`. Constructing this check should NOT trip any
  // static_assert statements!
  static_assert(!units<positive_fp_zero>);
  static_assert(!units<negative_fp_zero>);
  static_assert(!units<rzero>);
  static_assert(!units<positive_fp_zero_apples>);
  static_assert(!units<negative_fp_zero_apples>);
}

CONSTEXPR_TEST(MuUnits, PowInvalidExponent) {
// The following (rightly) does not compile:
#if 0
  using bad_apples = mu::pow<apples, 1, 0>;
#endif
}