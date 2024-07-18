#ifndef INCLUDED_MU_NPOW_HPP
#define INCLUDED_MU_NPOW_HPP
#include <mu/detail/factor.hpp>
#include <mu/detail/ratio.hpp>
#include <mu/detail/unit_string.hpp>
#include <mu/units.hpp>
#include <ratio>

namespace mu {

/// Represents an integer base raised to an integer power.
///
/// This enables represntation of scales that are too large or too small to fit
/// in a std::ratio.
///
/// \tparam BaseValue The base of the exponential expression. This value cannot
/// be 0.
/// \tparam Exponent The exponent.
///
/// \see mu::pow2 and \see mu::pow10 for convenient aliases of this template. If
/// a user wants an exponential value whose base is not 2 or 10, they can use
/// `npow` directly.
///
template <std::intmax_t BaseValue, std::intmax_t Exponent>
requires(BaseValue != 0)
struct npow {};

/// Represents a term that is a power of 2.
///
template <std::intmax_t Exponent> using pow2 = npow<2, Exponent>;

/// Represents a term that is a power of 10.
///
/// This enables representation of scales that are too large to fit in a
/// `std::ratio`. For example, a unit can be defined with a scale of
/// `pow10<999>` = `10⁹⁹⁹`. This could not possibly be stored in a `std::ratio`
/// because the numerator exceeds the limits of `intmax_t`. This does not
/// magically enable conversions between, say `meters` and `10⁹⁹⁹ meters`
/// because the conversion value cannot be represented in an integer or a
/// floating point value. However, a conversion between say `10⁹⁹⁹` and `10⁹⁹⁷`
/// is feasible because their scales differ only by a factor of `10² = 100`.
///
/// Note the string-formatting of these terms are `10ⁿ`. They never appear as
/// named prefixes (e.g. kilo, mega, giga).
///
template <std::intmax_t Exponent> using pow10 = npow<10, Exponent>;

namespace detail {

/// `npow` specializations are factors.
template <std::intmax_t BaseValue, std::intmax_t Exponent>
struct factor_traits<npow<BaseValue, Exponent>> {
  /// All instances of `npow<N, ...>` have  the same base type (`std::ratio<N>`)
  /// so that their exponents can be combined during dimensional analysis.
  using base = std::ratio<BaseValue>;
  constexpr static ratio exponent = Exponent;
  constexpr static bool is_dimensional = false;
  constexpr static bool is_rational_value = true;
  constexpr static ratio rational_value = BaseValue;
  constexpr static long double irrational_value = 1.0;
};

/// `npow` specializations are units.
template <std::intmax_t BaseValue, std::intmax_t Exponent>
struct unit_traits<npow<BaseValue, Exponent>> {
  /// Since `npow` specializations are factors, their factorization is a mult
  /// only containing itself.
  using factors = mu::mult<npow<BaseValue, Exponent>>;

  /// Formats the `npow` as `N^exponent`.
  constexpr static void format(unit_string &ustr) {
    ustr.push();
    ustr.multiply(ratio{BaseValue});
    ustr.pow(ratio{Exponent});
    ustr.pop();
  }
};

} // namespace detail

} // namespace mu

#endif