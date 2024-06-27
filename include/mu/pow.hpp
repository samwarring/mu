#ifndef INCLUDED_MU_POW_HPP
#define INCLUDED_MU_POW_HPP
#include <mu/detail/factor.hpp>
#include <mu/detail/ratio.hpp>
#include <mu/detail/to_string.hpp>
#include <mu/units.hpp>

namespace mu {

template <units Base, std::intmax_t ExpNum, std::intmax_t ExpDen = 1>
struct pow {};

namespace detail {

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

template <units Factors, std::intmax_t ExpNum, std::intmax_t ExpDen>
struct apply_pow {};

template <std::intmax_t ExpNum, std::intmax_t ExpDen, factor... Factors>
struct apply_pow<mult<Factors...>, ExpNum, ExpDen> {
  using type = mult<pow<Factors, ExpNum, ExpDen>...>;
};

template <units UnitsBase, std::intmax_t ExpNum, std::intmax_t ExpDen>
struct unit_traits<pow<UnitsBase, ExpNum, ExpDen>> {
  using factors = typename apply_pow<typename unit_traits<UnitsBase>::factors,
                                     ExpNum, ExpDen>::type;
  constexpr static std::string format(const format_options &opts) {
    std::string ret = unit_traits<UnitsBase>::format(opts);
    ret += "^" + to_string(ExpNum, ExpDen);
    return ret;
  }
};

} // namespace detail

} // namespace mu

#endif