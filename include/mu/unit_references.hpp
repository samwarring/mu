#ifndef INCLUDED_MU_UNIT_REFERENCES_HPP
#define INCLUDED_MU_UNIT_REFERENCES_HPP
#include <mu/pow.hpp>
#include <mu/quantity.hpp>
#include <mu/units.hpp>

/// NOTE: The following operators must be defined in the global namespace
/// because neither argument necessarily resides in the `mu` namespace, and thus
/// would not be discoverable by ADL.
///
/// ADL would suffice if the units were defined in the `mu` namespace (as would
/// be the case with the provided SI units), but in general, users can define
/// their own units in their own namespaces.

/// Multiplies two "reference" unit expressions to produce a new "reference"
/// unit.
///
/// The reference unit can be multiplied by raw values or quantities to produce
/// quantities of different units.
///
/// @tparam LhsUnits Reference units on the left-hand side of the operator.
/// @tparam RhsUnits Reference units on the right-hand side of the operator.
/// @return A new reference unit expression whose units are the product of
/// \p LhsUnits and \p RhsUnits.
///
template <mu::units LhsUnits, mu::units RhsUnits>
constexpr mu::mult<LhsUnits, RhsUnits> operator*(LhsUnits &&, RhsUnits &&) {
  return {};
}

/// Divides two "reference" unit expressions to produce a new "reference" unit.
///
/// The reference unit can be multiplied by raw values or quantities to produce
/// quantities of different units.
///
/// @tparam LhsUnits Reference units on the left-hand side of the operator.
/// @tparam RhsUnits Reference units on the right-hand side of the operator.
/// @return A new reference unit expression whose units are the product of
/// \p LhsUnits and \p RhsUnits.
///
template <mu::units LhsUnits, mu::units RhsUnits>
constexpr mu::mult<LhsUnits, mu::pow<RhsUnits, -1>> operator/(LhsUnits &&,
                                                              RhsUnits &&) {
  return {};
}

/// Multiplies a raw value by a unit reference, producing a quantity.
///
/// \tparam LhsRep Type of the raw value.
/// \tparam RhsUnits Reference unit expression.
/// \param value The raw value.
/// \return A new quantity whose value is the raw value, measured in the
/// reference units.
///
template <mu::rep LhsRep, mu::units RhsUnits>
constexpr mu::quantity<LhsRep, RhsUnits> operator*(LhsRep &&value,
                                                   RhsUnits &&) {
  return mu::quantity<LhsRep, RhsUnits>(std::forward<LhsRep>(value));
}

/// Multiplies a unit reference by a raw value, producing a quantity.
///
/// \tparam LhsUnits Reference unit expression.
/// \tparam RhsRep Type of the raw value.
/// \param value The raw value.
/// \return A new quantity whose value is the raw value, measured in the
/// reference units.
///
template <mu::units LhsUnits, mu::rep RhsRep>
constexpr mu::quantity<RhsRep, LhsUnits> operator*(LhsUnits &&,
                                                   RhsRep &&value) {
  return mu::quantity<RhsRep, LhsUnits>(std::forward<RhsRep>(value));
}

namespace mu {

/// Multiplies a quantity lvalue by a unit reference, producing a new quantity
/// of different units.
///
/// \tparam LhsRep Representation of the quantity on the left-hand side of the
/// operator.
/// \tparam LhsUnits Units of the quantity on the left-hand side of the
/// operator.
/// \tparam RhsUnits Unit reference on the right-hand side of the operator.
/// \param q The quantity on the left-hand side of the operator.
/// \return A new quantity whose value is the same as the input quantity, and
/// whose units are the product of the input quantity and the unit reference.
///
template <rep LhsRep, units LhsUnits, units RhsUnits>
constexpr quantity<LhsRep, mult<LhsUnits, RhsUnits>>
operator*(const quantity<LhsRep, LhsUnits> &q, RhsUnits &&) {
  return quantity<LhsRep, mult<LhsUnits, RhsUnits>>{q.value()};
}

/// Multiplies a quantity rvalue by a unit reference, producing a new quantity
/// of different units.
///
/// \tparam LhsRep Representation of the quantity on the left-hand side of the
/// operator.
/// \tparam LhsUnits Units of the quantity on the left-hand side of the
/// operator.
/// \tparam RhsUnits Unit reference on the right-hand side of the operator.
/// \param q The quantity on the left-hand side of the operator.
/// \return A new quantity whose value is the same as the input quantity, and
/// whose units are the product of the input quantity and the unit reference.
/// The input quantity's value is moved into the new quantity.
///
template <rep LhsRep, units LhsUnits, units RhsUnits>
constexpr quantity<LhsRep, mult<LhsUnits, RhsUnits>>
operator*(quantity<LhsRep, LhsUnits> &&q, RhsUnits &&) {
  return quantity<LhsRep, mult<LhsUnits, RhsUnits>>{std::move(q).value()};
}

/// Multiplies a unit reference by a quantity lvalue, producing a new quantity
/// of different units.
///
/// \tparam LhsUnits Unit reference on the left-hand side of the operator.
/// \tparam RhsRep Representation of the quantity on the right-hand side of the
/// operator.
/// \tparam RhsUnits Units of the quantity on the right-hand side of the
/// operator.
/// \param q The quantity on the right-hand side of the operator.
/// \return A new quantity whose value is the same as the input quantity, and
/// whose units are the product of the input quantity and the unit reference.
///
template <units LhsUnits, rep RhsRep, units RhsUnits>
constexpr quantity<RhsRep, mult<LhsUnits, RhsUnits>>
operator*(LhsUnits &&, const quantity<RhsRep, RhsUnits> &q) {
  return quantity<RhsRep, mult<LhsUnits, RhsUnits>>{q.value()};
}

/// Multiplies a unit reference by a quantity rvalue, producing a new quantity
/// of different units.
///
/// \tparam LhsUnits Unit reference on the left-hand side of the operator.
/// \tparam RhsRep Representation of the quantity on the right-hand side of the
/// operator.
/// \tparam RhsUnits Units of the quantity on the right-hand side of the
/// operator.
/// \param q The quantity on the right-hand side of the operator.
/// \return A new quantity whose value is the same as the input quantity, and
/// whose units are the product of the input quantity and the unit reference.
/// The input quantity's value is moved into the new quantity.
///
template <units LhsUnits, rep RhsRep, units RhsUnits>
constexpr quantity<RhsRep, mult<LhsUnits, RhsUnits>>
operator*(LhsUnits &&, quantity<RhsRep, RhsUnits> &&q) {
  return quantity<RhsRep, mult<LhsUnits, RhsUnits>>{std::move(q).value()};
}

/// Divides a quantity lvalue by a unit reference, producing a new quantity.
///
/// \tparam LhsRep Representation of the quantity on the left-hand side of the
/// operator.
/// \tparam LhsUnits Units of the quantity on the left-hand side of the
/// operator.
/// \tparam RhsUnits Unit reference on the right-hand side of the operator.
/// \param q The quantity on the left-hand side of the operator.
/// \return A new quantity whose value is the same as the input quantity, and
/// whose units are the quotient of the input quantity and the unit reference.
///
template <rep LhsRep, units LhsUnits, units RhsUnits>
constexpr quantity<LhsRep, mult<LhsUnits, pow<RhsUnits, -1>>>
operator/(const quantity<LhsRep, LhsUnits> &q, RhsUnits &&) {
  return quantity<LhsRep, mult<LhsUnits, pow<RhsUnits, -1>>>{q.value()};
}

/// Divides a quantity rvalue by a unit reference, producing a new quantity.
///
/// \tparam LhsRep Representation of the quantity on the left-hand side of the
/// operator.
/// \tparam LhsUnits Units of the quantity on the left-hand side of the
/// operator.
/// \tparam RhsUnits Unit reference on the right-hand side of the operator.
/// \param q The quantity on the left-hand side of the operator.
/// \return A new quantity whose value is the same as the input quantity, and
/// whose units are the quotient of the input quantity and the unit reference.
/// The input quantity's value is moved into the new quantity.
///
template <rep LhsRep, units LhsUnits, units RhsUnits>
constexpr quantity<LhsRep, mult<LhsUnits, pow<RhsUnits, -1>>>
operator/(quantity<LhsRep, LhsUnits> &&q, RhsUnits &&) {
  return quantity<LhsRep, mult<LhsUnits, pow<RhsUnits, -1>>>{
      std::move(q).value()};
}

} // namespace mu

#endif