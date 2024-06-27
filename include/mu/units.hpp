#ifndef INCLUDED_MU_UNITS_HPP
#define INCLUDED_MU_UNITS_HPP
#include <mu/detail/factor.hpp>
#include <string>
#include <tuple>
#include <type_traits>

namespace mu {

template <class... Ts> using mult = std::tuple<Ts...>;

namespace detail {

template <class... Mults>
using mult_concat_t = decltype(std::tuple_cat(std::declval<Mults>()...));

template <class T> struct is_factor_list : public std::false_type {};

template <factor... Factors>
struct is_factor_list<mult<Factors...>> : public std::true_type {};

template <class T> struct unit_traits {};

} // namespace detail

struct format_options {
  enum class label_type { names, symbols };
  label_type labels = label_type::names;
  const char *mult_sep = " * ";
};

template <class T>
concept units = requires(format_options fopts) {
  typename detail::unit_traits<T>::factors;
  requires detail::is_factor_list<
      typename detail::unit_traits<T>::factors>::value;
  { detail::unit_traits<T>::format(fopts) } -> std::convertible_to<std::string>;
};

namespace detail {

template <> struct unit_traits<mult<>> {
  using factors = mult<>;
  constexpr static std::string format(const format_options &) { return "1"; }
};

template <units UnitsHead, units... UnitsTail>
struct unit_traits<mult<UnitsHead, UnitsTail...>> {
  using factors = mult_concat_t<typename unit_traits<UnitsHead>::factors,
                                typename unit_traits<UnitsTail>::factors...>;
  constexpr static std::string format(const format_options &opts) {
    std::string ret;
    constexpr bool enclose = sizeof...(UnitsTail) > 0;
    if constexpr (enclose) {
      ret += "(";
    }
    ret += unit_traits<UnitsHead>::format(opts);
    ret += ((opts.mult_sep + unit_traits<UnitsTail>::format(opts)) + ...);
    if constexpr (enclose) {
      ret += ")";
    }
    return ret;
  }
};

} // namespace detail

} // namespace mu

#endif