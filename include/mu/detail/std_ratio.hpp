#ifndef INCLUDED_MU_DETAIL_STD_RATIO_HPP
#define INCLUDED_MU_DETAIL_STD_RATIO_HPP
#include <mu/detail/factor.hpp>
#include <mu/detail/ratio.hpp>
#include <mu/detail/unit_string.hpp>
#include <mu/units.hpp>
#include <ratio>

namespace mu::detail {

/// std::ratio can be a factor of a unit expression.
///
/// \tparam Num Numerator of the ratio.
/// \tparam Den Denominator of the ratio.
///
template <std::intmax_t Num, std::intmax_t Den>
struct factor_traits<std::ratio<Num, Den>> {
  using base = std::ratio<Num, Den>;
  constexpr static ratio exponent = 1;
  constexpr static bool is_dimensional = false;
  constexpr static bool is_rational_value = true;
  constexpr static ratio rational_value{Num, Den};
  constexpr static long double irrational_value = 1.0;
};

/// std::ratio can be used as a units expression representing a dimensionless
/// magnitude.
///
/// \tparam Num Numerator of the ratio.
/// \tparam Den Denominator of the ratio.
///
template <std::intmax_t Num, std::intmax_t Den>
struct unit_traits<std::ratio<Num, Den>> {
  /// The factors of a ratio include itself.
  using factors = mu::mult<std::ratio<Num, Den>>;

  /// Multiply the current expression by the ratio.
  constexpr static void format(unit_string &ustr) {
    ustr.multiply(ratio{Num, Den});
  }
};

} // namespace mu::detail

#endif