#include "mu_test.hpp"

using namespace mu;

template <units Units> constexpr std::string name() {
  format_options opts;
  opts.labels = format_options::label_type::names;
  opts.mult_sep = "*";
  opts.superscript_exponents = false;
  return to_string<Units>(opts);
}

template <units Units> constexpr std::string symbol() {
  format_options opts;
  opts.labels = format_options::label_type::symbols;
  opts.mult_sep = "*";
  opts.superscript_exponents = false;
  return to_string<Units>(opts);
}

CONSTEXPR_TEST(MuSiTest, UnitsConvertibleTo) {
  static_assert(units_convertible_to<meter, centimeter>);
}

TEST(MuSITest, Strings) {
  ASSERT_EQ(name<kilometer>(), "kilometer");
  ASSERT_EQ(symbol<microsecond>(), "µs");
  ASSERT_EQ(symbol<meter_per_second>(), "m*s^-1");
  ASSERT_EQ(symbol<milligram>(), "mg");
  ASSERT_EQ(symbol<kilogram>(), "kg");
}

TEST(MuSITest, Quantities) {
  quantity<int, hour> work_hours{8};
  quantity<int, second> work_seconds = work_hours;
  ASSERT_EQ(work_seconds.value(), 8 * 60 * 60);
}