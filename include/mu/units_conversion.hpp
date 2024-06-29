#ifndef INCLUDED_MU_UNITS_CONVERSION_HPP
#define INCLUDED_MU_UNITS_CONVERSION_HPP
#include <mu/detail/analysis.hpp>
#include <mu/units.hpp>

namespace mu {

namespace detail {

template <units FromUnits, units ToUnits> struct units_conversion {
  constexpr static auto value =
      analysis_object<FromUnits, ToUnits>.float_conversion;
};

template <units FromUnits, units ToUnits>
requires(detail::analysis_object<FromUnits, ToUnits>.is_int_convertible)
struct units_conversion<FromUnits, ToUnits> {
  constexpr static auto value =
      analysis_object<FromUnits, ToUnits>.int_conversion;
};

} // namespace detail

template <class FromUnits, class ToUnits>
concept units_convertible_to = requires {
  requires units<FromUnits>;
  requires units<ToUnits>;
  requires detail::analysis_object<FromUnits, ToUnits>.is_convertible;
};

template <class FromUnits, class ToUnits>
concept units_equivalent_to = requires {
  requires units<FromUnits>;
  requires units<ToUnits>;
  requires detail::analysis_object<FromUnits, ToUnits>.is_equivalent;
};

template <units FromUnits, units ToUnits>
requires units_convertible_to<FromUnits, ToUnits>
constexpr auto units_conversion_v =
    detail::units_conversion<FromUnits, ToUnits>::value;

template <units FromUnits, units ToUnits>
constexpr auto units_conversion_t =
    std::remove_cvref_t<decltype(units_conversion_v<FromUnits, ToUnits>)>;

} // namespace mu

#endif