#ifndef INCLUDED_MU_QUANTITY_HPP
#define INCLUDED_MU_QUANTITY_HPP
#include <iostream>
#include <mu/rep.hpp>
#include <mu/units.hpp>
#include <mu/units_conversion.hpp>

namespace mu {

/// Concept is `true` if a `quantity<FromRep, FromUnits>` can be stored in a
/// `quantity<ToRep, ToUnits>` without scaling the contained value, and without
/// loss of precision. For the operation to not require scaling, the units
/// `FromUnits` and `ToUnits` must be equivalent.
///
/// \tparam FromRep Representation of the source quantity.
/// \tparam FromUnits Units of the source quantity.
/// \tparam ToRep Representation of the destination quantity.
/// \tparam ToUnits Units of the destination quantity.
///
template <class FromRep, class FromUnits, class ToRep, class ToUnits>
concept quantity_losslessly_equivalent_to = requires {
  requires units_equivalent_to<FromUnits, ToUnits>;
  requires rep_losslessly_castable_to<FromRep, ToRep>;
};

/// Concept is `true` if a `quantity<FromRep, FromUnits>` can be stored in a
/// `quantity<ToRep, ToUnits>` without scaling the contained value, even if this
/// would result in a loss of precision. For the operation to not require
/// scaling, the units `FromUnits` and `ToUnits` must be equivalent.
///
/// \tparam FromRep Representation of the source quantity.
/// \tparam FromUnits Units of the source quantity.
/// \tparam ToRep Representation of the destination quantity.
/// \tparam ToUnits Units of the destination quantity.
///
template <class FromRep, class FromUnits, class ToRep, class ToUnits>
concept quantity_lossily_equivalent_to = requires {
  requires units_equivalent_to<FromUnits, ToUnits>;
  requires rep_lossily_castable_to<FromRep, ToRep>;
};

/// Concept is `true` if a `quantity<FromRep, FromUnits>` can be stored in a
/// `quantity<ToRep, ToUnits>` after scaling the source value to account for
/// the different units. The destination quantity must be capable of holding
/// the scaled value without loss of precision. The units `FromUnits` and
/// `ToUnits` need not be equivalent, but must share the same dimensions.
///
/// \tparam FromRep Representation of the source quantity.
/// \tparam FromUnits Units of the source quantity.
/// \tparam ToRep Representation of the destination quantity.
/// \tparam ToUnits Units of the destination quantity.
///
template <class FromRep, class FromUnits, class ToRep, class ToUnits>
concept quantity_losslessly_scalable_to = requires {
  requires units_convertible_to<FromUnits, ToUnits>;
  requires rep_losslessly_scalable_to<FromRep, ToRep,
                                      units_conversion_t<FromUnits, ToUnits>>;
};

/// Concept is `true` if a `quantity<FromRep, FromUnits>` can be stored in a
/// `quantity<ToRep, ToUnits>` after scaling the source value to account for
/// the different units. The destination quantity must be capable of holding
/// the scaled value, even if doing so would lose precision. The units
/// `FromUnits` and `ToUnits` need not be equivalent, but must share the same
/// dimensions.
///
/// \tparam FromRep Representation of the source quantity.
/// \tparam FromUnits Units of the source quantity.
/// \tparam ToRep Representation of the destination quantity.
/// \tparam ToUnits Units of the destination quantity.
///
template <class FromRep, class FromUnits, class ToRep, class ToUnits>
concept quantity_lossily_scalable_to = requires {
  requires units_convertible_to<FromUnits, ToUnits>;
  requires rep_lossily_scalable_to<FromRep, ToRep,
                                   units_conversion_t<FromUnits, ToUnits>>;
};

/// Concept is `true` if a `quantity<FromRep, FromUnits>` can be assigned to a
/// `quantity<ToRep, ToUnits>`. The assignment may or may not require scaling,
/// but the final value must be representable by `ToRep` without loss of
/// precision. The units `FromUnits` and `ToUnits` need not be equivalent, but
/// must share the same dimensions.
///
/// \tparam FromRep Representation of the source quantity.
/// \tparam FromUnits Units of the source quantity.
/// \tparam ToRep Representation of the destination quantity.
/// \tparam ToUnits Units of the destination quantity.
///
template <class FromRep, class FromUnits, class ToRep, class ToUnits>
concept quantity_losslessly_convertible_to = requires {
  requires rep<FromRep>;
  requires rep<ToRep>;
  requires units<FromUnits>;
  requires units<ToUnits>;
  requires quantity_losslessly_equivalent_to<FromRep, FromUnits, ToRep,
                                             ToUnits> or
               quantity_losslessly_scalable_to<FromRep, FromUnits, ToRep,
                                               ToUnits>;
};

/// Concept is `true` if a `quantity<FromRep, FromUnits>` can be assigned to a
/// `quantity<ToRep, ToUnits>`. The assignment may or may not require scaling,
/// but the final value must be representable by `ToRep` with or without loss of
/// precision. The units `FromUnits` and `ToUnits` need not be equivalent, but
/// must share the same dimensions.
///
/// \tparam FromRep Representation of the source quantity.
/// \tparam FromUnits Units of the source quantity.
/// \tparam ToRep Representation of the destination quantity.
/// \tparam ToUnits Units of the destination quantity.
///
template <class FromRep, class FromUnits, class ToRep, class ToUnits>
concept quantity_lossily_convertible_to = requires {
  requires rep<FromRep>;
  requires rep<ToRep>;
  requires units<FromUnits>;
  requires units<ToUnits>;
  requires quantity_lossily_equivalent_to<FromRep, FromUnits, ToRep, ToUnits> or
               quantity_lossily_scalable_to<FromRep, FromUnits, ToRep, ToUnits>;
};

/// Concept is `true` if a `quantity<FromRep, FromUnits>` can be assigned to a
/// `quantity<ToRep, ToUnits>`. The assignment may or may not require scaling,
/// but the final value must be representable by `ToRep` and *necessarily*
/// requires a loss of precision. The units `FromUnits` and `ToUnits` need not
/// be equivalent, but must share the same dimensions.
///
/// This concept is used to call out method overloads where a unit conversion
/// is possible but converting between the representation types would always
/// result in a loss of precision.
///
/// \tparam FromRep Representation of the source quantity.
/// \tparam FromUnits Units of the source quantity.
/// \tparam ToRep Representation of the destination quantity.
/// \tparam ToUnits Units of the destination quantity.
///
template <class FromRep, class FromUnits, class ToRep, class ToUnits>
concept quantity_only_lossily_convertible_to = requires {
  requires rep<FromRep>;
  requires rep<ToRep>;
  requires units<FromUnits>;
  requires units<ToUnits>;
  requires quantity_lossily_convertible_to<FromRep, FromUnits, ToRep, ToUnits>;
  requires !quantity_losslessly_convertible_to<FromRep, FromUnits, ToRep,
                                               ToUnits>;
};

namespace detail {

/// Convert one representation value to another, accounting for any difference
/// in units, without loss of precision. If no scaling is required, the result
/// is cast directly from the argument without any scalar multiplication.
///
/// \tparam ToRep Converting to this representation.
/// \tparam ToUnits Converting to these units.
/// \tparam FromUnits Converting from these units.
/// \tparam FromRep Converting from this representation. This template parameter
/// is last because it can usually be deduced from the argument type.
/// \param from_value The value to be converted.
///
template <rep ToRep, units ToUnits, units FromUnits, rep FromRep>
requires quantity_losslessly_convertible_to<FromRep, FromUnits, ToRep, ToUnits>
constexpr ToRep convert_losslessly(FromRep &&from_value) {
  if constexpr (units_equivalent_to<FromUnits, ToUnits>) {
    return rep_traits<ToRep>::lossless_cast(std::forward<FromRep>(from_value));

  } else {
    relaxed_scale_t<ToRep, units_conversion_t<FromUnits, ToUnits>> scale =
        units_conversion_v<FromUnits, ToUnits>;
    return rep_traits<ToRep>::lossless_cast(std::forward<FromRep>(from_value) *
                                            scale);
  }
}

/// Convert one representation value to another, accounting for any difference
/// in units, allowing any potential loss of precision. If no scaling is
/// required, the result is cast directly from the argument without any scalar
/// multiplication.
///
/// \tparam ToRep Converting to this representation.
/// \tparam ToUnits Converting to these units.
/// \tparam FromUnits Converting from these units.
/// \tparam FromRep Converting from this representation. This template parameter
/// is last because it can usually be deduced from the argument type.
/// \param from_value The value to be converted.
///
template <rep ToRep, units ToUnits, units FromUnits, rep FromRep>
requires quantity_lossily_convertible_to<FromRep, FromUnits, ToRep, ToUnits>
constexpr ToRep convert_lossily(FromRep &&from_value) {
  if constexpr (units_equivalent_to<FromUnits, ToUnits>) {
    return rep_traits<ToRep>::lossy_cast(std::forward<FromRep>(from_value));

  } else {
    relaxed_scale_t<ToRep, units_conversion_t<FromUnits, ToUnits>> scale =
        units_conversion_v<FromUnits, ToUnits>;
    return rep_traits<ToRep>::lossy_cast(std::forward<FromRep>(from_value) *
                                         scale);
  }
}

} // namespace detail

/// A value that is strongly-typed by its units of measurement.
///
/// A `quantity` wraps a *value*, which is an object of some
/// *representation* type. The representation can be any type, so long as it
/// satisfies the `rep` concept.
///
/// The units are are single type, or combination of types (combined by
/// `mu::mult` or `mu::pow`), that describe the scale and dimension of the
/// quantity's value. They serve only to enforce proper use of measured
/// quantities For example, they prevent assignment of `seconds` to `meters`,
/// and they deduce the needed conversion scale when assigning `seconds` to
/// `hours`. The quantity does not store any instaces of the `Units` type.
///
/// \tparam Rep The type of value wrapped by this quantity. Must satsify the
/// `rep` concept.
/// \tparam Units The units of measurement for this quantity. Must satsify the
/// `units` concept.
///
template <rep Rep, units Units> class quantity {
public:
  /// Alias for this quantity's representation type.
  using rep_type = Rep;

  /// Alias for this quantity's units type.
  using units_type = Units;

  /// Destruction of the quantity is exactly that of its representation.
  constexpr ~quantity() = default;

  /// Default construction of the quantity is exactly that of its
  /// representation.
  constexpr quantity() = default;

  /// Constructs the quantity from a raw unitless value.
  ///
  /// This constructor is explicit to encourage co-locating raw values with
  /// their explicit units of measurement.
  ///
  /// \param value The raw value.
  constexpr explicit quantity(Rep value) : value_{std::move(value)} {}

  /// Copy-constructs this quantity from another quantity with compatible units.
  ///
  /// The source quantity's value, after any potential conversion, must be
  /// representable in this quantity without loss of precision.
  ///
  /// \tparam FromRep Representation of the source quantity.
  /// \tparam FromUnits Units of the source quantity.
  /// \param from_quantity The source quantity.
  ///
  template <rep FromRep, units FromUnits>
  requires quantity_losslessly_convertible_to<FromRep, FromUnits, Rep, Units>
  constexpr quantity(const quantity<FromRep, FromUnits> &from_quantity)
      : value_{detail::convert_losslessly<Rep, Units, FromUnits>(
            from_quantity.value())} {}

  /// Deleted copy-constructor selected when the units are compatible but cannot
  /// be performed without loss of precision.
  ///
  /// To perform the conversion while acknowledging the loss of precision, use
  /// the `mu::quantity_cast` function.
  ///
  /// \tparam FromRep Representation of the source quantity.
  /// \tparam FromUnits Units of the source quantity.
  ///
  template <rep FromRep, units FromUnits>
  requires quantity_only_lossily_convertible_to<FromRep, FromUnits, Rep, Units>
  constexpr quantity(const quantity<FromRep, FromUnits> &) = delete;

  /// Deleted copy-constructor selected when the source quantity's units are not
  /// compatible with the destination's units.
  ///
  /// \tparam FromRep Representation of the source quantity.
  /// \tparam FromUnits Units of the source quantity.
  ///
  template <rep FromRep, units FromUnits>
  requires(!units_convertible_to<FromUnits, Units>)
  constexpr quantity(const quantity<FromRep, FromUnits> &) = delete;

  /// Move-constructs this quantity from another quantity with compatible units.
  ///
  /// The source quantity's value, after any potential conversion, must be
  /// representable in this quantity without loss of precision.
  ///
  /// \tparam FromRep Representation of the source quantity.
  /// \tparam FromUnits Units of the source quantity.
  /// \param from_quantity The source quantity.
  ///
  template <rep FromRep, units FromUnits>
  requires quantity_losslessly_convertible_to<FromRep, FromUnits, Rep, Units>
  constexpr quantity(quantity<FromRep, FromUnits> &&from_quantity)
      : value_{detail::convert_losslessly<Rep, Units, FromUnits>(
            std::move(from_quantity).value())} {}

  /// Deleted move-constructor selected when the units are compatible but cannot
  /// be performed without loss of precision.
  ///
  /// To perform the conversion while acknowledging the loss of precision, use
  /// the `quantity_cast` function.
  ///
  /// \tparam FromRep Representation of the source quantity.
  /// \tparam FromUnits Units of the source quantity.
  ///
  template <rep FromRep, units FromUnits>
  requires quantity_only_lossily_convertible_to<FromRep, FromUnits, Rep, Units>
  constexpr quantity(quantity<FromRep, FromUnits> &&) = delete;

  /// Deleted move-constructor selected when the source quantity's units are not
  /// compatible with the destination's units.
  ///
  /// \tparam FromRep Representation of the source quantity.
  /// \tparam FromUnits Units of the source quantity.
  ///
  template <rep FromRep, units FromUnits>
  requires(!units_convertible_to<FromUnits, Units>)
  constexpr quantity(quantity<FromRep, FromUnits> &&) = delete;

  /// Copy-assignment from another quantity.
  ///
  /// The other quantity's value, after any potential conversion, must be
  /// representable by this quantity's `Rep` type without loss of precision. To
  /// perform assignment that acknowledges loss of precision, use the
  /// `quantity_cast` function.
  ///
  /// \tparam FromRep Representation of the source quantity.
  /// \tparam FromUnits Units of the source quantity.
  /// \param from_quantity The source quantity.
  /// \return Reference to this quantity object.
  ///
  template <rep FromRep, units FromUnits>
  requires quantity_losslessly_convertible_to<FromRep, FromUnits, Rep, Units>
  constexpr quantity<Rep, Units> &
  operator=(const quantity<FromRep, FromUnits> &from_quantity) {
    value_ = detail::convert_losslessly<Rep, Units, FromUnits>(
        from_quantity.value());
    return *this;
  }

  /// Deleted copy-assignment selected when the units are compatible but the
  /// assignment would result in a loss of precision. To perform assignment that
  /// acknowledges loss of precision, use the `quantity_cast` function.
  ///
  /// \tparam FromRep Representation of the source quantity.
  /// \tparam FromUnits Units of the source quantity.
  ///
  template <rep FromRep, units FromUnits>
  requires quantity_only_lossily_convertible_to<FromRep, FromUnits, Rep, Units>
  constexpr quantity<Rep, Units> &
  operator=(const quantity<FromRep, FromUnits> &) = delete;

  /// Deleted copy-assignment selected when the units are not compatible but the
  /// assignment would result in a loss of precision.
  ///
  /// \tparam FromRep Representation of the source quantity.
  /// \tparam FromUnits Units of the source quantity.
  ///
  template <rep FromRep, units FromUnits>
  requires(!units_convertible_to<FromUnits, Units>)
  constexpr quantity<Rep, Units> &
  operator=(const quantity<FromRep, FromUnits> &) = delete;

  /// Move-assignment from another quantity.
  ///
  /// The other quantity's value, after any potential conversion, must be
  /// representable by this quantity's `Rep` type without loss of precision. To
  /// perform assignment that acknowledges loss of precision, use the
  /// `quantity_cast` function.
  ///
  /// \tparam FromRep Representation of the source quantity.
  /// \tparam FromUnits Units of the source quantity.
  /// \param from_quantity The source quantity.
  /// \return Reference to this quantity object.
  ///
  template <rep FromRep, units FromUnits>
  requires quantity_losslessly_convertible_to<FromRep, FromUnits, Rep, Units>
  constexpr quantity<Rep, Units> &
  operator=(quantity<FromRep, FromUnits> &&from_quantity) {
    value_ = detail::convert_losslessly<Rep, Units, FromUnits>(
        std::move(from_quantity).value());
    return *this;
  }

  /// Deleted move-assignment selected when the units are compatible but the
  /// assignment would result in a loss of precision. To perform assignment that
  /// acknowledges loss of precision, use the `quantity_cast` function.
  ///
  /// \tparam FromRep Representation of the source quantity.
  /// \tparam FromUnits Units of the source quantity.
  ///
  template <rep FromRep, units FromUnits>
  requires quantity_only_lossily_convertible_to<FromRep, FromUnits, Rep, Units>
  constexpr quantity<Rep, Units> &
  operator=(quantity<FromRep, FromUnits> &&) = delete;

  /// Deleted move-assignment selected when the units are not compatible but the
  /// assignment would result in a loss of precision.
  ///
  /// \tparam FromRep Representation of the source quantity.
  /// \tparam FromUnits Units of the source quantity.
  ///
  template <rep FromRep, units FromUnits>
  requires(!units_convertible_to<FromUnits, Units>)
  constexpr quantity<Rep, Units> &
  operator=(quantity<FromRep, FromUnits> &&) = delete;

  /// Returns the raw value contained in the quantity.
  ///
  constexpr Rep value() const & { return value_; }

  /// Returns the raw value contained in the quantity.
  ///
  /// This overload is selected when the quantity is being moved. Accordingly,
  /// the result is moved out of this quantity.
  ///
  constexpr Rep &&value() && { return std::move(value_); }

private:
  /// The raw value.
  Rep value_;
};

/// Converts one quantity to another, acknowledging that the conversion may
/// result in a loss of precision.
///
/// The source and destination units must be compatible with one another. That
/// is, they may only differ by a constant scale factor.
///
/// \tparam ToRep Representation of the destination quantity.
/// \tparam ToUnits Units of the destination quantity.
/// \tparam FromRep Representation of the source quantity.
/// \tparam FromUnits Units of the source quantity.
/// \param from_quantity The source quantity, as an lvalue.
/// \return The destination quantity, properly scaled to account for the
/// difference in units. Note that precision may have been lost during the
/// conversion.
///
template <rep ToRep, units ToUnits, rep FromRep, units FromUnits>
requires quantity_lossily_convertible_to<FromRep, FromUnits, ToRep, ToUnits>
constexpr quantity<ToRep, ToUnits>
quantity_cast(const quantity<FromRep, FromUnits> &from_quantity) {
  return quantity<ToRep, ToUnits>{
      detail::convert_lossily<ToRep, ToUnits, FromUnits>(
          from_quantity.value())};
}

/// Overload of `quantity_cast` selected when a user tries to cast quantities
/// of incompatible units, and the argument is an lvalue.
///
/// \tparam ToRep Representation of the destination quantity.
/// \tparam ToUnits Units of the destination quantity.
/// \tparam FromRep Representation of the source quantity.
/// \tparam FromUnits Units of the source quantity.
///
template <rep ToRep, units ToUnits, rep FromRep, units FromUnits>
requires(!units_convertible_to<FromUnits, ToUnits>)
constexpr quantity<ToRep, ToUnits>
quantity_cast(const quantity<FromRep, FromUnits> &) = delete;

/// Converts one quantity to another, acknowledging that the conversion may
/// result in a loss of precision.
///
/// The source and destination units must be compatible with one another. That
/// is, they may only differ by a constant scale factor.
///
/// \tparam ToRep Representation of the destination quantity.
/// \tparam ToUnits Units of the destination quantity.
/// \tparam FromRep Representation of the source quantity.
/// \tparam FromUnits Units of the source quantity.
/// \param from_quantity The source quantity, as an rvalue.
/// \return The destination quantity, properly scaled to account for the
/// difference in units. Note that precision may have been lost during the
/// conversion.
///
template <rep ToRep, units ToUnits, rep FromRep, units FromUnits>
requires quantity_lossily_convertible_to<FromRep, FromUnits, ToRep, ToUnits>
constexpr quantity<ToRep, ToUnits>
quantity_cast(quantity<FromRep, FromUnits> &&from_quantity) {
  return quantity<ToRep, ToUnits>{
      detail::convert_lossily<ToRep, ToUnits, FromUnits>(
          std::move(from_quantity).value())};
}

/// Overload of `quantity_cast` selected when a user tries to cast quantities
/// of incompatible units, and the argument is an rvalue.
///
/// \tparam ToRep Representation of the destination quantity.
/// \tparam ToUnits Units of the destination quantity.
/// \tparam FromRep Representation of the source quantity.
/// \tparam FromUnits Units of the source quantity.
///
template <rep ToRep, units ToUnits, rep FromRep, units FromUnits>
requires(!units_convertible_to<FromUnits, ToUnits>)
constexpr quantity<ToRep, ToUnits>
quantity_cast(quantity<FromRep, FromUnits> &&from_quantity) = delete;

/// Compares quantities for equality.
///
/// The quantities must have equivalent units, because it may be ambiguous what
/// common units are needed for the comparison, and because any implicit
/// conversion may be expensive for the `Rep` types.
///
/// \tparam LhsRep Representation of quantity on the left-hand side.
/// \tparam LhsUnits Units of quantity on the left-hand side.
/// \tparam RhsRep Representation of quantity on the right-hand side.
/// \tparam RhsUnits Units of quantity on the right-hand side.
/// \param lhs Quantity on the left-hand side of the comparison.
/// \param rhs Quantity on the right-hand side of the comparison.
/// \return True if the quantities' values are equal.
///
template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
requires units_equivalent_to<LhsUnits, RhsUnits>
constexpr bool operator==(const quantity<LhsRep, LhsUnits> &lhs,
                          const quantity<RhsRep, RhsUnits> &rhs) {
  return lhs.value() == rhs.value();
}

/// Compares quantities for inequality.
///
/// The quantities must have equivalent units, because it may be ambiguous what
/// common units are needed for the comparison, and because any implicit
/// conversion may be expensive for the `Rep` types.
///
/// \tparam LhsRep Representation of quantity on the left-hand side.
/// \tparam LhsUnits Units of quantity on the left-hand side.
/// \tparam RhsRep Representation of quantity on the right-hand side.
/// \tparam RhsUnits Units of quantity on the right-hand side.
/// \param lhs Quantity on the left-hand side of the comparison.
/// \param rhs Quantity on the right-hand side of the comparison.
/// \return True if the quantities' values are not equal.
///
template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
requires units_equivalent_to<LhsUnits, RhsUnits>
constexpr bool operator!=(const quantity<LhsRep, LhsUnits> &lhs,
                          const quantity<RhsRep, RhsUnits> &rhs) {
  return lhs.value() != rhs.value();
}

/// Compares quantities using less-than.
///
/// The quantities must have equivalent units, because it may be ambiguous what
/// common units are needed for the comparison, and because any implicit
/// conversion may be expensive for the `Rep` types.
///
/// \tparam LhsRep Representation of quantity on the left-hand side.
/// \tparam LhsUnits Units of quantity on the left-hand side.
/// \tparam RhsRep Representation of quantity on the right-hand side.
/// \tparam RhsUnits Units of quantity on the right-hand side.
/// \param lhs Quantity on the left-hand side of the comparison.
/// \param rhs Quantity on the right-hand side of the comparison.
/// \return True if the value of \p lhs is less than the value of \p rhs.
///
template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
requires units_equivalent_to<LhsUnits, RhsUnits>
constexpr bool operator<(const quantity<LhsRep, LhsUnits> &lhs,
                         const quantity<RhsRep, RhsUnits> &rhs) {
  return lhs.value() < rhs.value();
}

/// Compares quantities using less-than-or-equal.
///
/// The quantities must have equivalent units, because it may be ambiguous what
/// common units are needed for the comparison, and because any implicit
/// conversion may be expensive for the `Rep` types.
///
/// \tparam LhsRep Representation of quantity on the left-hand side.
/// \tparam LhsUnits Units of quantity on the left-hand side.
/// \tparam RhsRep Representation of quantity on the right-hand side.
/// \tparam RhsUnits Units of quantity on the right-hand side.
/// \param lhs Quantity on the left-hand side of the comparison.
/// \param rhs Quantity on the right-hand side of the comparison.
/// \return True if the value of \p lhs is less than or equal to the value of
/// \p rhs.
///
template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
requires units_equivalent_to<LhsUnits, RhsUnits>
constexpr bool operator<=(const quantity<LhsRep, LhsUnits> &lhs,
                          const quantity<RhsRep, RhsUnits> &rhs) {
  return lhs.value() <= rhs.value();
}

/// Compares quantities using greater-than.
///
/// The quantities must have equivalent units, because it may be ambiguous what
/// common units are needed for the comparison, and because any implicit
/// conversion may be expensive for the `Rep` types.
///
/// \tparam LhsRep Representation of quantity on the left-hand side.
/// \tparam LhsUnits Units of quantity on the left-hand side.
/// \tparam RhsRep Representation of quantity on the right-hand side.
/// \tparam RhsUnits Units of quantity on the right-hand side.
/// \param lhs Quantity on the left-hand side of the comparison.
/// \param rhs Quantity on the right-hand side of the comparison.
/// \return True if the value of \p lhs is greater than the value of \p rhs.
///
template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
requires units_equivalent_to<LhsUnits, RhsUnits>
constexpr bool operator>(const quantity<LhsRep, LhsUnits> &lhs,
                         const quantity<RhsRep, RhsUnits> &rhs) {
  return lhs.value() > rhs.value();
}

/// Compares quantities using greater-than-or-equal.
///
/// The quantities must have equivalent units, because it may be ambiguous what
/// common units are needed for the comparison, and because any implicit
/// conversion may be expensive for the `Rep` types.
///
/// \tparam LhsRep Representation of quantity on the left-hand side.
/// \tparam LhsUnits Units of quantity on the left-hand side.
/// \tparam RhsRep Representation of quantity on the right-hand side.
/// \tparam RhsUnits Units of quantity on the right-hand side.
/// \param lhs Quantity on the left-hand side of the comparison.
/// \param rhs Quantity on the right-hand side of the comparison.
/// \return True if the value of \p lhs is greater than or equal to the value of
/// \p rhs.
///
template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
requires units_equivalent_to<LhsUnits, RhsUnits>
constexpr bool operator>=(const quantity<LhsRep, LhsUnits> &lhs,
                          const quantity<RhsRep, RhsUnits> &rhs) {
  return lhs.value() >= rhs.value();
}

/// Negates a quantity.
///
/// \tparam Rep Representation of the source quantity.
/// \tparam Units Units of the source quantity.
/// \param q The source quantity.
/// \return A quantity with the same units of the source quantity, whose value
/// is the negation of the source quantity. The representation of the resulting
/// quantity may be different from the source quantity if negating the source
/// value yields a different type.
///
template <rep Rep, units Units>
constexpr auto operator-(const quantity<Rep, Units> &q) {
  return quantity<Rep, Units>(-q.value());
}

/// Adds two quantities.
///
/// The quantities must have equivalent units, because it may be ambiguous what
/// common units are needed for the addition, and because any implicit
/// conversion may be expensive for the `Rep` types.
///
/// \tparam LhsRep Representation of quantity on the left-hand side.
/// \tparam LhsUnits Units of quantity on the left-hand side.
/// \tparam RhsRep Representation of quantity on the right-hand side.
/// \tparam RhsUnits Units of quantity on the right-hand side.
/// \param lhs Quantity on the left-hand side of the operator.
/// \param rhs Quantity on the right-hand side of the operator.
/// \return A quantity whose value is the sum of the input quantities' values.
/// Its representation is deduced as the type obtained by adding the quantities'
/// raw values. Its units are that of the quantity on the left-hand side.
///
template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
requires units_equivalent_to<LhsUnits, RhsUnits> && rep_addable<LhsRep, RhsRep>
constexpr auto operator+(const quantity<LhsRep, LhsUnits> &lhs,
                         const quantity<RhsRep, RhsUnits> &rhs) {
  auto sum_value = lhs.value() + rhs.value();
  using sum_rep = std::remove_cvref_t<decltype(sum_value)>;
  return quantity<sum_rep, LhsUnits>{std::move(sum_value)};
}

/// Subtracts one quantity from another.
///
/// The quantities must have equivalent units, because it may be ambiguous what
/// common units are needed for the addition, and because any implicit
/// conversion may be expensive for the `Rep` types.
///
/// \tparam LhsRep Representation of quantity on the left-hand side.
/// \tparam LhsUnits Units of quantity on the left-hand side.
/// \tparam RhsRep Representation of quantity on the right-hand side.
/// \tparam RhsUnits Units of quantity on the right-hand side.
/// \param lhs Quantity on the left-hand side of the operator.
/// \param rhs Quantity on the right-hand side of the operator.
/// \return A quantity whose value is the difference of the input quantities'
/// values. Its representation is deduced as the type obtained by subtracting
/// the quantities' raw values. Its units are that of the quantity on the
/// left-hand side.
///
template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
requires units_equivalent_to<LhsUnits, RhsUnits> &&
         rep_subtractable<LhsRep, RhsRep>
constexpr auto operator-(const quantity<LhsRep, LhsUnits> &lhs,
                         const quantity<RhsRep, RhsUnits> &rhs) {
  auto difference_value = lhs.value() - rhs.value();
  using difference_rep = std::remove_cvref_t<decltype(difference_value)>;
  return quantity<difference_rep, LhsUnits>{std::move(difference_value)};
}

/// Multiplies two quantities.
///
/// \tparam LhsRep Representation of quantity on the left-hand side.
/// \tparam LhsUnits Units of quantity on the left-hand side.
/// \tparam RhsRep Representation of quantity on the right-hand side.
/// \tparam RhsUnits Units of quantity on the right-hand side.
/// \param lhs Quantity on the left-hand side of the operator.
/// \param rhs Quantity on the right-hand side of the operator.
/// \return A quantity whose value is the product of the input quantities'
/// values. Its representation is deduced as the type obtained by multiplying
/// the quantities' raw values. Its units are the product of the the input
/// quantities' units.
///
template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
requires rep_multiplicable<LhsRep, RhsRep>
constexpr auto operator*(const quantity<LhsRep, LhsUnits> &lhs,
                         const quantity<RhsRep, RhsUnits> &rhs) {
  auto product_value = lhs.value() * rhs.value();
  using product_rep = std::remove_cvref_t<decltype(product_value)>;
  using product_units = mu::mult<LhsUnits, RhsUnits>;
  return quantity<product_rep, product_units>{std::move(product_value)};
}

/// Multiplies a quantity by a raw value, thus scaling the quantity.
///
/// \tparam LhsRep Representation of quantity on the left-hand side.
/// \tparam LhsUnits Units of quantity on the left-hand side.
/// \tparam RhsType Type of the raw value.
/// \param lhs Quantity on the left-hand side of the operator.
/// \param rhs Raw value on the right-hand side of the operator.
/// \return A quantity whose value is the product of the quantity's raw value
/// and the additional raw value. Its representation is deduced as the type
/// obtained by multiplying the raw values. Its units are the same as the input
/// quantity.
///
template <rep LhsRep, units LhsUnits, class RhsType>
requires rep_multiplicable<LhsRep, RhsType>
constexpr auto operator*(const quantity<LhsRep, LhsUnits> &lhs, RhsType &&rhs) {
  auto product_value = lhs.value() * std::forward<RhsType>(rhs);
  using product_rep = std::remove_cvref_t<decltype(product_value)>;
  return quantity<product_rep, LhsUnits>{std::move(product_value)};
}

/// Multiplies a raw value by a quantity, thus scaling the quantity.
///
/// \tparam LhsType Type of the raw value.
/// \tparam RhsRep Representation of quantity on the right-hand side.
/// \tparam RhsUnits Units of quantity on the right-hand side.
/// \param lhs Raw value on the left-hand side of the operator.
/// \param rhs Quantity on the right-hand side of the operator.
/// \return A quantity whose value is the product of the quantity's raw value
/// and the additional raw value. Its representation is deduced as the type
/// obtained by multiplying the raw values. Its units are the same as the input
/// quantity.
///
template <class LhsType, rep RhsRep, units RhsUnits>
requires rep_multiplicable<LhsType, RhsRep>
constexpr auto operator*(LhsType &&lhs, const quantity<RhsRep, RhsUnits> &rhs) {
  auto product_value = std::forward<LhsType>(lhs) * rhs.value();
  using product_rep = std::remove_cvref_t<decltype(product_value)>;
  return quantity<product_rep, RhsUnits>{std::move(product_value)};
}

/// Divides two quantities.
///
/// \tparam LhsRep Representation of quantity on the left-hand side.
/// \tparam LhsUnits Units of quantity on the left-hand side.
/// \tparam RhsRep Representation of quantity on the right-hand side.
/// \tparam RhsUnits Units of quantity on the right-hand side.
/// \param lhs Quantity on the left-hand side of the operator.
/// \param rhs Quantity on the right-hand side of the operator.
/// \return A quantity whose value is the product of the input quantities'
/// values. Its representation is deduced as the type obtained by dividing
/// the quantities' raw values. Its units are the quotient of the the input
/// quantities' units.
///
template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
requires rep_dividable<LhsRep, RhsRep>
constexpr auto operator/(const quantity<LhsRep, LhsUnits> &lhs,
                         const quantity<RhsRep, RhsUnits> &rhs) {
  auto quotient_value = lhs.value() / rhs.value();
  using quotient_rep = std::remove_cvref_t<decltype(quotient_value)>;
  using quotient_units = mu::mult<LhsUnits, mu::pow<RhsUnits, -1>>;
  return quantity<quotient_rep, quotient_units>{std::move(quotient_value)};
}

/// Divides a quantity by a raw value, thus scaling the quantity.
///
/// \tparam LhsRep Representation of quantity on the left-hand side.
/// \tparam LhsUnits Units of quantity on the left-hand side.
/// \tparam RhsType Type of the raw value.
/// \param lhs Quantity on the left-hand side of the operator.
/// \param rhs Raw value on the right-hand side of the operator.
/// \return A quantity whose value is the quotient of the quantity's raw value
/// and the additional raw value. Its representation is deduced as the type
/// obtained by dividing the raw values. Its units are the same as the input
/// quantity.
///
template <rep LhsRep, units LhsUnits, class RhsType>
requires rep_dividable<LhsRep, RhsType>
constexpr auto operator/(const quantity<LhsRep, LhsUnits> &lhs, RhsType &&rhs) {
  auto quotient_value = lhs.value() / std::forward<RhsType>(rhs);
  using quotient_rep = std::remove_cvref_t<decltype(quotient_value)>;
  return quantity<quotient_rep, LhsUnits>{std::move(quotient_value)};
}

/// Divides a raw value by a quantity.
///
/// \tparam LhsType Type of the raw value.
/// \tparam RhsRep Representation of quantity on the right-hand side.
/// \tparam RhsUnits Units of quantity on the right-hand side.
/// \param lhs Raw value on the left-hand side of the operator.
/// \param rhs Quantity on the right-hand side of the operator.
/// \return A quantity whose value is the quotient of the raw value and the
/// quantity's value. Its representation is deduced as the type obtained by
/// dividing the raw values. Its units are the inverse of the input quantity.
///
template <class LhsType, rep RhsRep, units RhsUnits>
requires rep_dividable<LhsType, RhsRep>
constexpr auto operator/(LhsType &&lhs, const quantity<RhsRep, RhsUnits> &rhs) {
  auto quotient_value = std::forward<LhsType>(lhs) / rhs.value();
  using quotient_rep = std::remove_cvref_t<decltype(quotient_value)>;
  return quantity<quotient_rep, mu::pow<RhsUnits, -1>>{
      std::move(quotient_value)};
}

} // namespace mu

#endif