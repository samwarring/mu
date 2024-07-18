#include "mu_test.hpp"

namespace /* local to this file only */ {
constexpr mu::detail::labels a{"unita", "a", false};
constexpr mu::detail::labels b{"unitb", "b", false};
constexpr mu::detail::labels c{"unitc", "c", false};
constexpr mu::detail::labels d{"unitd", "d", false};
constexpr mu::detail::labels m{"meters", "m", false};
constexpr mu::detail::labels k{"kilo", "k", true};

constexpr auto label_type = mu::format_options::label_type::symbols;
constexpr mu::format_options fopts{label_type, " * "};
} // namespace

struct MuUnitString : public testing::Test {
  mu::detail::unit_string ustr{fopts};
};

TEST_F(MuUnitString, Empty) { ASSERT_EQ(ustr.str(), ""); }

TEST_F(MuUnitString, OneSymbol) {
  ustr.multiply(a);
  ASSERT_EQ(ustr.str(), "a");
}

TEST_F(MuUnitString, TwoSymbols) {
  ustr.multiply(a);
  ustr.multiply(b);
  ASSERT_EQ(ustr.str(), "a * b");
}

TEST_F(MuUnitString, MultExprTimesMultExpr) {
  ustr.push();
  ustr.multiply(a);
  ustr.multiply(b);
  ustr.pop();
  ustr.push();
  ustr.multiply(c);
  ustr.multiply(d);
  ustr.pop();
  ASSERT_EQ(ustr.str(), "a * b * c * d");
}

TEST_F(MuUnitString, NestedMult) {
  ustr.push();
  ustr.multiply(a);
  ustr.push();
  ustr.multiply(b);
  ustr.push();
  ustr.multiply(c);
  ustr.pop();
  ustr.pop();
  ustr.pop();
  ASSERT_EQ(ustr.str(), "a * b * c");
}

TEST_F(MuUnitString, IntegerPow) {
  ustr.multiply(a);
  ustr.pow(2);
  ASSERT_EQ(ustr.str(), "a^2");
}

TEST_F(MuUnitString, PowProductExpression) {
  ustr.multiply(a);
  ustr.multiply(b);
  ustr.pow(3);
  ASSERT_EQ(ustr.str(), "(a * b)^3");
}

TEST_F(MuUnitString, PrefixedSymbol) {
  ustr.multiply(k);
  ustr.multiply(m);
  ASSERT_EQ(ustr.str(), "km");
}

TEST_F(MuUnitString, PrefixedConstant) {
  ustr.multiply(k);
  ustr.multiply(24);
  ASSERT_EQ(ustr.str(), "k * 24");
}

TEST_F(MuUnitString, PrefixedSymbolPower) {
  ustr.multiply(k);
  ustr.multiply(m);
  ustr.pow(4);
  ASSERT_EQ(ustr.str(), "km^4");
}

CONSTEXPR_TEST(MuUnitString, ConstexprUsage) {
  constexpr std::string make_str() {
    mu::detail::unit_string ustr(fopts);
    ustr.multiply(a);
    ustr.push();
    ustr.multiply(b);
    ustr.pow(2);
    ustr.pop();
    return ustr.str();
  }
  static_assert(make_str() == "a * b^2");
}

TEST_F(MuUnitString, SuperscriptExponent) {
  mu::format_options opts;
  opts.labels = mu::format_options::label_type::symbols;
  opts.superscript_exponents = true;
  mu::detail::unit_string ustr2{opts};
  ustr2.multiply(a);
  mu::detail::ratio exponent{-12345, 67890};
  ustr2.pow(exponent);
  ASSERT_EQ(ustr2.str(), "a⁻¹²³⁴⁵ᐟ⁶⁷⁸⁹⁰");
}

TEST_F(MuUnitString, Npow) {
  using u = mu::npow<7, 99>;
  mu::detail::unit_traits<u>::format(ustr);
  ASSERT_EQ(ustr.str(), "7^99");
}