#ifndef INCLUDED_MU_DETAIL_SYMBOLS_HPP
#define INCLUDED_MU_DETAIL_SYMBOLS_HPP
#include <concepts>
#include <mu/detail/factor.hpp>
#include <mu/detail/ratio.hpp>
#include <mu/units.hpp>

/// \file sybmols.hpp
///
/// This file contains core concepts needed to define base units and base
/// constants, as well as composite units and composite constants.

namespace mu::detail {

/// Concept is `true` if the type defines static members for its name and
/// symbol.
template <class T>
concept has_labels = requires {
  { T::name } -> std::convertible_to<const char *>;
  { T::symbol } -> std::convertible_to<const char *>;
};

/// Helper class that can format types if they provide label members.
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

/// Concept is `true` if the type defines a floating point `value` member.
template <class T>
concept has_value = requires {
  { T::value } -> std::convertible_to<long double>;
};

/// Concept is `true` if the type defines a nested `units` type.
template <class T>
concept has_units = requires { typename T::units; };

/// Concept matches types that represent base units of a unit expression. These
/// are units that are not defined in terms of other units.
template <class T>
concept base_unit = requires {
  requires has_labels<T>;
  requires !has_value<T>;
  requires !has_units<T>;
};

/// base_unit types are factors.
template <base_unit BaseUnit> struct factor_traits<BaseUnit> {
  using base = BaseUnit;
  constexpr static ratio exponent = 1;
  constexpr static bool is_dimensional = true;
  constexpr static bool is_rational_value = true;
  constexpr static ratio rational_value = 1;
  constexpr static long double irrational_value = 1.0;
};

/// base_unit types are units.
template <base_unit BaseUnit>
struct unit_traits<BaseUnit> : public label_formatter<BaseUnit> {
  using factors = mult<BaseUnit>;
};

/// Concept matches types that represent symbolic constants of a unit
/// expression. These constants have no units. They are intended to be defined
/// for constants that cannot be represented by a rational value (e.g. pi), so
/// they must define a static `value` member that holds an approximation of its
/// irrational value. Even though the value is an approximation, dimensional
/// analysis is capable of "canceling out" base constants where it can to keep
/// calculations accurate and precise.
template <class T>
concept base_constant = requires {
  requires has_labels<T>;
  requires has_value<T>;
  requires !has_units<T>;
};

/// base_constant types are factors.
template <base_constant BaseConstant> struct factor_traits<BaseConstant> {
  using base = BaseConstant;
  constexpr static ratio exponent = 1;
  constexpr static bool is_dimensional = false;
  constexpr static bool is_rational_value = false;
  constexpr static ratio rational_value = 1;
  constexpr static long double irrational_value = BaseConstant::value;
};

/// base_constant types are units.
template <base_constant BaseConstant>
struct unit_traits<BaseConstant> : public label_formatter<BaseConstant> {
  using factors = mult<BaseConstant>;
};

/// Concept matches types that describe a named unit defined in terms of other
/// units. When formatting a unit expression, composite units are formatted
/// according to their own name, not according to the names of their constiuent
/// units.
template <class T>
concept composite_unit = requires {
  requires has_labels<T>;
  requires !has_value<T>;
  requires has_units<T>;
  requires units<typename T::units>;
};

/// composite_unit types are units.
template <composite_unit CompositeUnit>
struct unit_traits<CompositeUnit> : public label_formatter<CompositeUnit> {
  using factors = typename unit_traits<typename CompositeUnit::units>::factors;
};

/// Concept matches types that describe a dimensional constant. These are
/// constants that cannot be represented by a rational number, and are also
/// measured according to other units. An example of this type of unit would be
/// the gravitational constant `G` defined as `6.67430e-11` with units
/// `N • m² / kg²`.
template <class T>
concept composite_constant = requires {
  requires has_labels<T>;
  requires has_value<T>;
  requires has_units<T>;
  requires units<typename T::units>;
};

/// To factorize a composite_constant, we define a type that represents only the
/// magnitude of the composite constant. This is always included in the
/// factorization of a composite constant, in addition to the factorization of
/// the constant's units.
template <composite_constant CompositeConstant>
struct composite_constant_value {};

/// The type representing the magnitude of a composite constant is a factor.
template <composite_constant CompositeConstant>
struct factor_traits<composite_constant_value<CompositeConstant>> {
  using base = composite_constant_value<CompositeConstant>;
  constexpr static ratio exponent = 1;
  constexpr static bool is_dimensional = false;
  constexpr static bool is_rational_value = false;
  constexpr static ratio rational_value = 1;
  constexpr static long double irrational_value = CompositeConstant::value;
};

/// The type representing the magnitude of a composite constant is a unit
/// expression.
template <composite_constant CompositeConstant>
struct unit_traits<composite_constant_value<CompositeConstant>> {
  using factors = mult<composite_constant_value<CompositeConstant>>;
  constexpr static std::string format(const format_options &opts) {
    // NOTE: We expect this method will never be called in practice.
    return "|" + label_formatter<CompositeConstant>::format(opts) + "|";
  }
};

/// composite_constant types are units.
template <composite_constant CompositeConstant>
struct unit_traits<CompositeConstant>
    : public label_formatter<CompositeConstant> {
  using factors = mult_concat_t<
      mu::mult<composite_constant_value<CompositeConstant>>,
      typename unit_traits<typename CompositeConstant::units>::factors>;
};

} // namespace mu::detail

#endif