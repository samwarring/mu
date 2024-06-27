#ifndef INCLUDED_MU_DETAIL_SYMBOLS_HPP
#define INCLUDED_MU_DETAIL_SYMBOLS_HPP
#include <concepts>
#include <mu/detail/factor.hpp>
#include <mu/detail/ratio.hpp>
#include <mu/units.hpp>

namespace mu::detail {

template <class T>
concept has_labels = requires {
  { T::name } -> std::convertible_to<const char *>;
  { T::symbol } -> std::convertible_to<const char *>;
};

template <has_labels HasLabels> struct label_formatter {
  constexpr static std::string format(const format_options opts) {
    switch (opts.labels) {
    case format_options::label_type::names:
      return HasLabels::name;
    case format_options::label_type::symbols:
      return HasLabels::symbol;
    default:
      return HasLabels::name;
    }
  }
};

template <class T>
concept has_value = requires {
  { T::value } -> std::convertible_to<long double>;
};

template <class T>
concept has_units = requires { typename T::units; };

template <class T>
concept base_unit = requires {
  requires has_labels<T>;
  requires !has_value<T>;
  requires !has_units<T>;
};

template <base_unit BaseUnit> struct factor_traits<BaseUnit> {
  using base = BaseUnit;
  constexpr static ratio exponent = 1;
  constexpr static bool is_dimensional = true;
  constexpr static bool is_rational_value = true;
  constexpr static ratio rational_value = 1;
  constexpr static long double irrational_value = 1.0;
};

template <base_unit BaseUnit>
struct unit_traits<BaseUnit> : public label_formatter<BaseUnit> {
  using factors = mult<BaseUnit>;
};

template <class T>
concept base_constant = requires {
  requires has_labels<T>;
  requires has_value<T>;
  requires !has_units<T>;
};

template <base_constant BaseConstant> struct factor_traits<BaseConstant> {
  using base = BaseConstant;
  constexpr static ratio exponent = 1;
  constexpr static bool is_dimensional = false;
  constexpr static bool is_rational_value = false;
  constexpr static ratio rational_value = 1;
  constexpr static long double irrational_value = BaseConstant::value;
};

template <base_constant BaseConstant>
struct unit_traits<BaseConstant> : public label_formatter<BaseConstant> {
  using factors = mult<BaseConstant>;
};

template <class T>
concept composite_unit = requires {
  requires has_labels<T>;
  requires !has_value<T>;
  requires has_units<T>;
  requires units<typename T::units>;
};

template <composite_unit CompositeUnit>
struct unit_traits<CompositeUnit> : public label_formatter<CompositeUnit> {
  using factors = typename unit_traits<typename CompositeUnit::units>::factors;
};

template <class T>
concept composite_constant = requires {
  requires has_labels<T>;
  requires has_value<T>;
  requires has_units<T>;
  requires units<typename T::units>;
};

template <composite_constant CompositeConstant>
struct composite_constant_value {};

template <composite_constant CompositeConstant>
struct factor_traits<composite_constant_value<CompositeConstant>> {
  using base = composite_constant_value<CompositeConstant>;
  constexpr static ratio exponent = 1;
  constexpr static bool is_dimensional = false;
  constexpr static bool is_rational_value = false;
  constexpr static ratio rational_value = 1;
  constexpr static long double irrational_value = CompositeConstant::value;
};

template <composite_constant CompositeConstant>
struct unit_traits<composite_constant_value<CompositeConstant>> {
  using factors = mult<composite_constant_value<CompositeConstant>>;
  constexpr static std::string format(const format_options &opts) {
    // NOTE: We expect this method will never be called in practice.
    return "|" + label_formatter<CompositeConstant>::format(opts) + "|";
  }
};

template <composite_constant CompositeConstant>
struct unit_traits<CompositeConstant>
    : public label_formatter<CompositeConstant> {
  using factors = mult_concat_t<
      mu::mult<composite_constant_value<CompositeConstant>>,
      typename unit_traits<typename CompositeConstant::units>::factors>;
};

} // namespace mu::detail

#endif