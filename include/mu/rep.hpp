#ifndef INCLUDED_MU_REP_HPP
#define INCLUDED_MU_REP_HPP
#include <concepts>
#include <limits>

namespace mu {

/// Representation conversion traits are responsible for identifying the safety
/// of converting from values of `FromRep` to values of `ToRep`.
///
/// If the conversion can be performed without loss of precision (or if by the
/// nature of `ToRep`, loss of precision is acceptable), then this class defines
/// a static bool `is_convertible` with value `true`. Otherwise, the value is
/// `false`.
///
/// The traits are also responsible for providing a static `convert` method that
/// performs the conversion without warnings from the compiler.
///
/// If `FromRep` and `ToRep` are fundamental C++ types, this library defines all
/// necessary specalizations of this template. To adapt custom types for use
/// with this library, integrators will likely need to write additional
/// specializations of this template.
///
template <class FromRep, class ToRep> struct rep_conversion_traits {

  /// By default, we can perform the conversion if brace-initialization from
  /// `FromRep` to `ToRep` is well-formed.
  ///
  constexpr static bool is_convertible =
      requires(FromRep from_value) { ToRep{from_value}; };

  /// Convert from `FromRep` to `ToRep` using brace-initialization.
  ///
  /// If this is not well-formed, we expect a compiler error. If constructing a
  /// quantity from a custom representation type fails to compile, the user can
  /// write a new specialization of `rep_conversion_traits` that checks for
  /// convertibility and performs the conversion.
  ///
  constexpr static ToRep convert(FromRep &&from_value) {
    return ToRep{std::forward<FromRep>(from_value)};
  }
};

template <std::integral FromIntRep, std::floating_point ToFloatRep>
struct rep_conversion_traits<FromIntRep, ToFloatRep> {

  /// If the mantissa of the floating-point destination type is wide enough to
  /// fit all the bits of the source integer type, then we can safely convert
  /// source to destination without any loss of precision.
  ///
  constexpr static bool is_convertible =
      std::numeric_limits<FromIntRep>::digits <=
      std::numeric_limits<ToFloatRep>::digits;

  /// Despite C++ declaring all conversions from integer to floating point a
  /// "narrowing conversion", we already checked if the destination is wide
  /// enough to safely store the source value (see `is_convertible`). We can
  /// force the compiler to allow this conversion with `static_cast`.
  ///
  constexpr static ToFloatRep convert(FromIntRep &&from_value) {
    return static_cast<ToFloatRep>(std::forward<FromIntRep>(from_value));
  }
};

/// Concept is true if `FromRep` can be converted to `ToRep` without inadvertent
/// loss of precision.
///
template <class FromRep, class ToRep>
concept rep_convertible_to =
    rep_conversion_traits<FromRep, ToRep>::is_convertible;

} // namespace mu

#endif