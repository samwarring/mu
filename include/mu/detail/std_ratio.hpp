#ifndef INCLUDED_MU_DETAIL_STD_RATIO_HPP
#define INCLUDED_MU_DETAIL_STD_RATIO_HPP
#include <mu/detail/factor.hpp>
#include <mu/detail/to_string.hpp>
#include <mu/units.hpp>
#include <ratio>

namespace mu::detail {

template <std::intmax_t Num, std::intmax_t Den>
struct factor_traits<std::ratio<Num, Den>> {
  using base = std::ratio<Num, Den>;
  constexpr static ratio exponent = 1;
  constexpr static bool is_dimensional = false;
  constexpr static bool is_rational_value = true;
  constexpr static ratio rational_value{Num, Den};
  constexpr static long double irrational_value = 1.0;
};

template <std::intmax_t Num, std::intmax_t Den>
struct unit_traits<std::ratio<Num, Den>> {
  using factors = mu::mult<std::ratio<Num, Den>>;
  constexpr static std::string format(const format_options &) {
    return to_string(Num, Den);
  }
};

} // namespace mu::detail

#endif