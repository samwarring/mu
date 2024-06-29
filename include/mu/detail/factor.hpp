#ifndef INCLUDED_MU_DETAIL_FACTOR_HPP
#define INCLUDED_MU_DETAIL_FACTOR_HPP
#include <mu/detail/ratio.hpp>

namespace mu::detail {

/// Defines the necessary traits for a type to match the `factor` concept.
///
/// For details on the necessary members, see the `factor` concept.
///
/// \tparam T Define factor characteristics for this type.
///
template <class T> struct factor_traits {};

/// Concept evaluates `true` if `T` represents a factor of a unit expression.
///
/// Factor types are a subset of units types. All unit types can be factorized
/// into a list of factors, but the factorization of a factor type is a list
/// containing itself. Simply put, factors are units that cannot be decomposed
/// into constituent units. Even more simply put, they are the "atoms" of unit
/// expressions.
///
template <class T>
concept factor = requires {
  // Every factor defines a base type. Factors with the same base type can be
  // combined during dimensional analysis; otherwise, they are distinct.
  typename factor_traits<T>::base;

  // Every factor has a rational exponent.
  { factor_traits<T>::exponent } -> std::convertible_to<ratio>;

  // Some factors have dimensionality (e.g. meters, seconds, grams). Other
  // factors do not (e.g. pi, 1/2).
  { factor_traits<T>::is_dimensional } -> std::convertible_to<bool>;

  // If the factor does not have dimensionality, it must have a magnitude. This
  // member is `true` if the magnitude of the base is representable by a ratio.
  // Note this only considers if the *base* is rational; it ignores any
  // irrationality caused by a fractional exponent.
  { factor_traits<T>::is_rational_value } -> std::convertible_to<bool>;

  // If the factor is not dimensional, and its base magnitude is representable
  // by a ratio, this is the base's rational value.
  { factor_traits<T>::rational_value } -> std::convertible_to<ratio>;

  // If the factor is not dimensional, and its base magnitude is not
  // representable by a ratio, this is the base's irrational value.
  { factor_traits<T>::irrational_value } -> std::convertible_to<long double>;
};

} // namespace mu::detail

#endif