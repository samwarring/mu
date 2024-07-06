#ifndef INCLUDED_MU_REP_HPP
#define INCLUDED_MU_REP_HPP
#include <concepts>
#include <limits>

namespace mu {

/// Fundamental C++ data types that can represent quantity values.
///
template <class T>
concept base_rep = std::integral<T> || std::floating_point<T>;

/// For a type to be allowed as a quantity representation, there must be a
/// specalization of this traits template that implements the type's lossy and
/// lossless conversions. See the `rep` concept for all requirements.
///
/// Note the library already provides a specialization for the fundamental C++
/// types that satisfy `base_rep`.
///
/// \tparam T Define traits for this representation type.
///
template <class T> struct rep_traits {};

/// Concept matches types that can represent quantity values.
///
/// To satsify the concept, there must be a specialization `rep_traits<T>` that
/// defines the following:
///
///   1. A member type `base_rep_type` that satisfies `base_rep`. This member
///      type declares the precision characteristics of the rep type.
///
///   2. A static method `lossless_cast` that consumes a generic `FromType` and
///      produces a `T`. If the method is not defined for a specific `FromType`,
///      then that means a `T` could not be converted from a `FromType` - either
///      because the types have nothing to do with each other, or because doing
///      so would result in a loss of precision. The method must accept
///      `FromType` by lvalue or by rvalue, and it must provide an
///      implementation where `FromType = T`, because a `T` is certainly
///      losslessly constructable from another `T`.
///
///   3. A static method `lossy_cast` that consumes a generic `FromType` and
///      produces a `T`. This method must be implemented even if converting a
///      `FromType` to a `T` would result in a loss of precision. If the method
///      is not defined for some `FromType`, then it means `T` and `FromType`
///      are completely unrelated clasess, and conversion between them makes no
///      sense. The method must accept `FromType` by lvalue or by rvalue, and it
///      must provide an implementation where `FromType = T`, because a `T` is
///      certainly lossily constructable from another `T`.
///
template <class T>
concept rep = requires(T lvalue, T &&rvalue) {
  requires base_rep<typename rep_traits<T>::base_rep_type>;
  { rep_traits<T>::lossless_cast(lvalue) } -> std::same_as<T>;
  { rep_traits<T>::lossless_cast(std::move(rvalue)) } -> std::same_as<T>;
  { rep_traits<T>::lossy_cast(lvalue) } -> std::same_as<T>;
  { rep_traits<T>::lossy_cast(std::move(rvalue)) } -> std::same_as<T>;
};

namespace detail {

/// Concept matches `rep` types whose precision is characterized by a floating
/// point type.
///
template <class Rep>
concept floating_point_rep = requires {
  requires rep<Rep>;
  requires std::floating_point<typename rep_traits<Rep>::base_rep_type>;
};

/// Concept evaluates `true` if `To` can be brace-initialized by a `From`.
///
template <class From, class To>
concept brace_convertible_to = requires(From from_value) { To{from_value}; };

/// Concept evaluates `true` if `From` is an integral type that can be fully
/// representable by a `To` floating-point type without loss of precision.
///
/// Note that C++ defines any conversion from integral types to floating-point
/// types as a "narrowing conversion" - even conversions from `int8_t` to
/// `double`. This concept inspects the types to see if such a cast truly
/// results in loss of precision.
///
template <class From, class To>
concept int_fits_in_float = requires {
  requires std::integral<From>;
  requires std::floating_point<To>;
  requires std::numeric_limits<From>::digits <= std::numeric_limits<To>::digits;
};

/// Actual type used to scale to a `ToRep`.
///
/// If the conversion between units is a floating point value, the scale itself
/// is a `long double` to prevent losing as much information as possible.
/// However, if all floating-point conversions were performed with `long
/// double`, then the resulting data-type would never be wide enough to hold the
/// scaled value unless the resulting data-type itself were a `long double`.
///
/// To address this, if the resulting data-type is a floating-point value, the
/// scale is casted - or "relaxed" - to the resulting data-type, so the
/// conversion would appear to not lose any precision. If neither the resulting
/// data-type nor the scale is a floating-point value, then the unmodified scale
/// is used, and no relaxation takes place.
///
/// \tparam ToRep Representation type that holds the result of a scaling
/// conversion.
/// \tparam Scale Type of the scale during a scaling conversion.
///
template <class ToRep, class Scale>
using relaxed_scale_t = std::conditional_t<
    /* if   */ floating_point_rep<ToRep> && std::floating_point<Scale>,
    /* then */ typename rep_traits<ToRep>::base_rep_type,
    /* else */ Scale>;

} // namespace detail

/// Specialization of `rep_traits` for the allowed fundamental C++ types.
///
template <base_rep BaseRep> struct rep_traits<BaseRep> {
  using base_rep_type = BaseRep;

  template <base_rep FromRep>
  requires detail::brace_convertible_to<FromRep, BaseRep>
  constexpr static BaseRep lossless_cast(FromRep from_value) {
    return BaseRep{from_value};
  }

  template <std::integral FromIntRep>
  requires detail::int_fits_in_float<FromIntRep, BaseRep>
  constexpr static BaseRep lossless_cast(FromIntRep from_value) {
    return static_cast<BaseRep>(from_value);
  }

  template <base_rep FromRep>
  constexpr static BaseRep lossy_cast(FromRep from_value) {
    return static_cast<BaseRep>(from_value);
  }
};

/// Concept is `true` if `ToRep` can be constructed from a `FromRep` without
/// losing precision. Such capability is determined by the `rep_traits<ToRep>`
/// specialization.
///
/// \tparam FromRep Constructing from this `rep`.
/// \tparam ToRep Constructing an instance of this `rep`.
///
template <class FromRep, class ToRep>
concept rep_losslessly_castable_to = requires(FromRep from_value) {
  requires rep<FromRep>;
  requires rep<ToRep>;
  { rep_traits<ToRep>::lossless_cast(from_value) } -> std::same_as<ToRep>;
};

/// Concept is `true` if `ToRep` can be constructed from `FromRep`, even if this
/// results in a loss of precision. Such capability is determined by the
/// `rep_traits<ToRep>` specialization.
///
/// \tparam FromRep Constructing from this `rep`
/// \tparam ToRep Constructing an instance of this `rep`.
///
template <class FromRep, class ToRep>
concept rep_lossily_castable_to = requires(FromRep from_value) {
  requires rep<FromRep>;
  requires rep<ToRep>;
  { rep_traits<ToRep>::lossy_cast(from_value) } -> std::same_as<ToRep>;
};

/// Concept is `true` if scaling `FromRep` by a `Scale` can be stored in a
/// `ToRep` without loss of precision. Such capability is determined by the
/// `rep_traits<ToRep>` specialization.
///
/// \tparam FromRep The original `rep` type holding the pre-scaled value.
/// \tparam Scale The type of the scale factor (may be integral or floating
/// point). Note the scale type itself may be "relaxed" prior to multiplication.
/// \tparam ToRep The resulting `rep` type that holds the post-scaled value.
///
template <class FromRep, class ToRep, class Scale>
concept rep_losslessly_scalable_to = requires(
    FromRep from_value, detail::relaxed_scale_t<ToRep, Scale> scale_value) {
  requires rep<FromRep>;
  requires rep<ToRep>;
  requires rep_losslessly_castable_to<FromRep, ToRep>;
  requires rep_losslessly_castable_to<decltype(from_value * scale_value),
                                      ToRep>;
};

/// Concept is `true` if scaling `FromRep` by a `Scale` can be stored in a
/// `ToRep` even if this would lose precision. Such capability is determined by
/// the `rep_traits<ToRep>` specialization.
///
/// \tparam FromRep The original `rep` type holding the pre-scaled value.
/// \tparam Scale The type of the scale factor (may be integral or floating
/// point). Note the scale type itself may be "relaxed" prior to multiplication.
/// \tparam ToRep The resulting `rep` type that holds the post-scaled value.
///
template <class FromRep, class ToRep, class Scale>
concept rep_lossily_scalable_to = requires(
    FromRep from_value, detail::relaxed_scale_t<ToRep, Scale> scale_value) {
  requires rep<FromRep>;
  requires rep<ToRep>;
  requires rep_lossily_castable_to<FromRep, ToRep>;
  requires rep_lossily_castable_to<decltype(from_value * scale_value), ToRep>;
};

} // namespace mu

#endif