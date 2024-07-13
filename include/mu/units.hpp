#ifndef INCLUDED_MU_UNITS_HPP
#define INCLUDED_MU_UNITS_HPP
#include <mu/detail/factor.hpp>
#include <mu/detail/unit_string.hpp>
#include <string>
#include <tuple>
#include <type_traits>

namespace mu {

/// Represents a product of unit expressions.
///
/// \tparam ...Ts Each type in the list must conform to the units concept. If
/// this type list is empty, this represents the multiplicative identity (i.e.
/// `1`)
///
template <class... Ts> using mult = std::tuple<Ts...>;

namespace detail {

/// Concatenates several product expressions into a single product expression.
///
/// \tparam ...Mults Each type in the list must be a specialization of mult<>.
///
template <class... Mults>
using mult_concat_t = decltype(std::tuple_cat(std::declval<Mults>()...));

/// Contains a static bool member called `value` that is `true` when T is a
/// specialization of mult, and each type in the mult matches the `factor`
/// concept.
///
/// \tparam T This must be a specialization of mult.
/// \see mu::detail::factor
///
template <class T> struct is_factor_list : public std::false_type {};

template <factor... Factors>
struct is_factor_list<mult<Factors...>> : public std::true_type {};

/// Defines the characteristics of types that represent units.
///
/// To specialize this template such that T matches the units concept, see \ref
/// mu::units.
///
/// \tparam T Define units characteristics for this type.
///
template <class T> struct unit_traits {};

} // namespace detail

/// The units concept matches types that can represent units of dimensional
/// analysis.
///
/// The following types match the units concept:
///  1. Named unit types (e.g. meters, kilograms, etc.)
///  2. Named constant types that define approximate values (e.g. pi)
///  3. `std::ratio` types
///  4. `mu::mult<...>` specializations, where each type in the `mult`
///     specialization also matches the units concept.
///  5. `mu::pow<...>` specializations, where the base type of the `pow` also
///     matches the units concept.
///
template <class T>
concept units = requires(format_options fopts) {
  // Each units type must provide a factorization.
  typename detail::unit_traits<T>::factors;

  // The type of the above factorization must be a mult<> of types matching the
  // `factor` concept.
  requires detail::is_factor_list<
      typename detail::unit_traits<T>::factors>::value;

  // Each units type must provide a static `format` member that takes a
  // `format_options` and returns a string.
  { detail::unit_traits<T>::format(fopts) } -> std::convertible_to<std::string>;
};

namespace detail {

/// Unit traits for an empty product expression (i.e. `1`).
template <> struct unit_traits<mult<>> {

  /// Empty product has no factors.
  using factors = mult<>;

  /// Formatting an empty product displays "1".
  constexpr static std::string format(const format_options &) { return "1"; }
};

/// Unit traits for a non-empty product expression.
///
/// \tparam UnitsHead The first unit of the product.
/// \tparam ...UnitsTail The remaining units of the product.
///
template <units UnitsHead, units... UnitsTail>
struct unit_traits<mult<UnitsHead, UnitsTail...>> {

  /// Factors of a product expression are the concatenation of all
  /// factorizations of each unit in the product expression.
  using factors = mult_concat_t<typename unit_traits<UnitsHead>::factors,
                                typename unit_traits<UnitsTail>::factors...>;

  /// Formatting a product expression displays each unit in the expression,
  /// joined by `format_options::mult_sep`. If the product contains more than
  /// one unit, the entire string is enclosed in parentheses.
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