#ifndef INCLUDED_MU_POW_HPP
#define INCLUDED_MU_POW_HPP
#include <mu/detail/factor.hpp>
#include <mu/detail/ratio.hpp>
#include <mu/detail/unit_string.hpp>
#include <mu/units.hpp>

namespace mu {

/// The pow template represents a unit expression raised to a rational power.
///
/// \tparam Base A base unit expression.
/// \tparam ExpNum The numerator of the exponent.
/// \tparam ExpDen The denominator of the exponent.
///
template <units Base, std::intmax_t ExpNum, std::intmax_t ExpDen = 1>
struct pow {};

namespace detail {

/// If the base of a `pow` matches the factor concept, then the resulting `pow`
/// is also a factor. The exponent of the resulting factor is modified.
///
template <factor FactorBase, std::intmax_t ExpNum, std::intmax_t ExpDen>
struct factor_traits<pow<FactorBase, ExpNum, ExpDen>> {
  using base = typename factor_traits<FactorBase>::base;
  constexpr static ratio exponent =
      factor_traits<FactorBase>::exponent * ratio{ExpNum, ExpDen};
  constexpr static bool is_dimensional =
      factor_traits<FactorBase>::is_dimensional;
  constexpr static bool is_rational_value =
      factor_traits<FactorBase>::is_rational_value;
  constexpr static ratio rational_value =
      factor_traits<FactorBase>::rational_value;
  constexpr static long double irrational_value =
      factor_traits<FactorBase>::irrational_value;
};

/// Wraps each `Ts` in a `mult<Ts...>` inside a `pow`.
///
/// Example:
/// `apply_pow< mult<a, b, c>, 2 >` == `mult< pow<a,2>, pow<b,2>, pow<c,2> >`
///
template <units Factors, std::intmax_t ExpNum, std::intmax_t ExpDen>
struct apply_pow {};

template <std::intmax_t ExpNum, std::intmax_t ExpDen, factor... Factors>
struct apply_pow<mult<Factors...>, ExpNum, ExpDen> {
  using type = mult<pow<Factors, ExpNum, ExpDen>...>;
};

/// If the base of a `pow` matches the units concept, then the entire `pow`
/// matches the units concept.
///
/// \tparam UnitsBase The base of the pow. Must match `units` concept.
/// \tparam ExpNum The numerator of the exponent.
/// \tparam ExpDen The denominator of the exponent.
///
template <units UnitsBase, std::intmax_t ExpNum, std::intmax_t ExpDen>
struct unit_traits<pow<UnitsBase, ExpNum, ExpDen>> {
  /// Factors of a pow are the factors of the base, except each factor is
  /// wrapped in a pow with the same exponent.
  using factors = typename apply_pow<typename unit_traits<UnitsBase>::factors,
                                     ExpNum, ExpDen>::type;

  /// Push a new subexpression containing the formatted base raised to a power.
  constexpr static void format(unit_string &ustr) {
    ustr.push();
    unit_traits<UnitsBase>::format(ustr);
    ustr.pow(ratio{ExpNum, ExpDen});
    ustr.pop();
  }
};

} // namespace detail

} // namespace mu

#endif