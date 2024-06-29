#ifndef INCLUDED_MU_UNITS_CONVERSION_HPP
#define INCLUDED_MU_UNITS_CONVERSION_HPP
#include <mu/detail/analysis.hpp>
#include <mu/units.hpp>

namespace mu {

namespace detail {

/// A template struct that defines the multiplier needed to convert `FromUnits`
/// to `ToUnits`. The value may be an integer, or a floating point value.
///
/// By default, use the floating point conversion value determined by unit
/// analysis.
///
/// \tparam FromUnits Converting from these units.
/// \tparam ToUnits Converting to these units.
///
template <units FromUnits, units ToUnits> struct units_conversion {
  constexpr static auto value =
      analysis_object<FromUnits, ToUnits>.float_conversion;
};

/// Specialization of `units_conversion` when the conversion can be performed by
/// multiplying by an integer.
///
/// \tparam FromUnits Converting from these units.
/// \tparam ToUnits Converting to these units.
///
template <units FromUnits, units ToUnits>
requires(detail::analysis_object<FromUnits, ToUnits>.is_int_convertible)
struct units_conversion<FromUnits, ToUnits> {
  constexpr static auto value =
      analysis_object<FromUnits, ToUnits>.int_conversion;
};

} // namespace detail

/// Concept evaluates `true` when `FromUnits` and `ToUnits` measure the same
/// dimensions. That is, they are dimensionally equivalent.
///
/// Examples of convertible units:
///  1. units_convertible_to<meters, feet>
///  2. units_convertible_to<meters_per_second, miles_per_hour>
///  3. units_convertible_to<newton_meters, joule_minutes>
///
/// Examples of units that are NOT convertible:
///  1. units_convertible_to<meters, seconds>
///  2. units_convertible_to<meters, square_meters>
///  3. units_convertible_to<watts, joules>
///
/// \tparam FromUnits Converting from these units. Must match `units` concept.
/// \tparam ToUnits Converting to these units. Must match `units` concept.
///
template <class FromUnits, class ToUnits>
concept units_convertible_to = requires {
  requires units<FromUnits>;
  requires units<ToUnits>;
  requires detail::analysis_object<FromUnits, ToUnits>.is_convertible;
};

/// Concept evaluates `true` when `FromUnits` and `ToUnits` are exactly
/// equivalent. That is, the conversion from `FromUnits` to `ToUnits` is exactly
/// `1`. This may be the case even if the units are derived from different
/// expressions. Equivalent units are convertible by definition.
///
/// Examples of equivalent units:
///  1. units_equivalent_to<meters, meters>
///  2. units_equivalent_to<newton_meters, watt_seconds>
///
/// Examples of units that are NOT equivalent:
///  1. units_equivalent_to<meters, feet>
///  2. units_equivalent_to<meters, square_meters>
///
/// \tparam FromUnits Converting from these units. Must match `units` concept.
/// \tparam ToUnits Converting to these units. Must match `units` concept.
///
template <class FromUnits, class ToUnits>
concept units_equivalent_to = requires {
  requires units<FromUnits>;
  requires units<ToUnits>;
  requires detail::analysis_object<FromUnits, ToUnits>.is_equivalent;
};

/// A conversion value that, when multiplied by a quantity measured in
/// `FromUnits`, produces an equivalent value measured in `ToUnits`.
///
/// \tparam FromUnits Converting from these units. Must match `units` concept.
/// \tparam ToUnits Converting to these units. Must match `units` concept.
///
template <units FromUnits, units ToUnits>
requires units_convertible_to<FromUnits, ToUnits>
constexpr auto units_conversion_v =
    detail::units_conversion<FromUnits, ToUnits>::value;

/// The type of `units_conversion_v<...>` for the given units. The conversion
/// may be an integral or a floating-point type.
///
/// \tparam FromUnits Converting from these units. Must match `units` concept.
/// \tparam ToUnits Converting to these units. Must match `units` concept.
///
template <units FromUnits, units ToUnits>
constexpr auto units_conversion_t =
    std::remove_cvref_t<decltype(units_conversion_v<FromUnits, ToUnits>)>;

} // namespace mu

#endif