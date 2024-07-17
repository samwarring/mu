#ifndef INCLUDED_MU_UNITS_CONVERSION_HPP
#define INCLUDED_MU_UNITS_CONVERSION_HPP
#include <mu/detail/analysis.hpp>
#include <mu/units.hpp>

namespace mu {

namespace detail {

/// A template struct that defines the multiplier needed to convert `FromUnits`
/// to `ToUnits`. The value may be an integer, or a floating point value.
///
/// If the conversion value is an integer, this struct stores the value using
/// the narrowest possible type. If the conversion value is not an integer, the
/// struct stores the value as a `long double`.
///
/// \tparam FromUnits Converting from these units.
/// \tparam ToUnits Converting to these units.
///
template <units FromUnits, units ToUnits> struct units_conversion {
private:
  constexpr static auto &ao = analysis_object<FromUnits, ToUnits>;
  constexpr static bool fits_int8 =
      ao.int_conversion >= std::numeric_limits<int8_t>::min() &&
      ao.int_conversion <= std::numeric_limits<int8_t>::max();
  constexpr static bool fits_int16 =
      ao.int_conversion >= std::numeric_limits<int16_t>::min() &&
      ao.int_conversion <= std::numeric_limits<int16_t>::max();
  constexpr static bool fits_int32 =
      ao.int_conversion >= std::numeric_limits<int32_t>::min() &&
      ao.int_conversion <= std::numeric_limits<int32_t>::max();
  constexpr static bool fits_int64 =
      ao.int_conversion >= std::numeric_limits<int64_t>::min() &&
      ao.int_conversion <= std::numeric_limits<int64_t>::max();

public:
  // clang-format off
  using type = std::conditional_t<
      /* if   */ !ao.is_int_convertible,        /* then */ long double,
      /* elif */ std::conditional_t<fits_int8,  /* then */ std::int8_t,
      /* elif */ std::conditional_t<fits_int16, /* then */ std::int16_t,
      /* elif */ std::conditional_t<fits_int32, /* then */ std::int32_t,
      /* elif */ std::conditional_t<fits_int64, /* then */ std::int64_t,
      /* else */ std::intmax_t>>>>>;
  // clang-format on

  constexpr static auto value = static_cast<type>(
      ao.is_int_convertible ? ao.int_conversion : ao.float_conversion);
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
using units_conversion_t =
    typename detail::units_conversion<FromUnits, ToUnits>::type;

} // namespace mu

#endif