#include "mu_test.hpp"
#include <mu/mu.hpp>

CONSTEXPR_TEST(MuRep, RepConversionTraitsIntToInt) {
  using t = mu::rep_conversion_traits<int, int>;
  static_assert(t::is_convertible);
  static_assert(t::convert(1) == 1);
}

CONSTEXPR_TEST(MuRep, RepConversionTraitsShortToInt) {
  using t = mu::rep_conversion_traits<short, int>;
  static_assert(t::is_convertible);
  static_assert(t::convert((short)1) == 1);
}

CONSTEXPR_TEST(MuRep, RepConversionTraitsShortToFloat) {
  using t = mu::rep_conversion_traits<short, float>;
  static_assert(t::is_convertible);
  static_assert(t::convert((short)1) == 1.0f);
}

CONSTEXPR_TEST(MuRep, RepConversionTraitsIntToDouble) {
  using t = mu::rep_conversion_traits<int, double>;
  static_assert(t::is_convertible);
  static_assert(t::convert(1) == 1.0);
}

CONSTEXPR_TEST(MuRep, RepConversionTraitsFloatToDouble) {
  using t = mu::rep_conversion_traits<float, double>;
  static_assert(t::is_convertible);
  static_assert(t::convert(1.0f) == 1.0);
}

CONSTEXPR_TEST(MuRep, RepConversionTraitsDoubleToFloat) {
  using t = mu::rep_conversion_traits<double, float>;
  static_assert(!t::is_convertible);
}

CONSTEXPR_TEST(MuRep, RepConversionTraitsIntToFloat) {
  using t = mu::rep_conversion_traits<int, float>;
  static_assert(!t::is_convertible);
}