#ifndef INCLUDED_MU_DETAIL_CONCRETE_FACTOR_HPP
#define INCLUDED_MU_DETAIL_CONCRETE_FACTOR_HPP
#include <array>
#include <limits>
#include <mu/detail/factor.hpp>
#include <mu/detail/ratio.hpp>
#include <mu/units.hpp>

namespace mu::detail {

/// Identifies the base type of a factor. Concrete factors with the same base_id
/// can be combined during dimensional analysis.
using concrete_factor_base_id = std::size_t;

/// Represents a base type for which there is no id.
constexpr concrete_factor_base_id UNKNOWN_BASE_ID =
    std::numeric_limits<concrete_factor_base_id>::max();

/// Holds a member for each value defined in mu::detail::factor_traits. Most
/// members map over to this structure obviously. However, the factor_traits for
/// a factor type T, define a member type `base`. The concrete factor cannot
/// store a type as a member, but it can store a unique id for that type. The id
/// is assigned by the concrete_factor_generator type.
///
struct concrete_factor {
  concrete_factor_base_id base_id = UNKNOWN_BASE_ID;
  ratio exponent = 0;
  bool is_dimensional = false;
  bool is_rational_value = 1;
  ratio rational_value = 1;
  long double irrational_value = 1.0;
};

/// Factory that can generate an array of concrete factors from a type-list of
/// factor types. The resulting array is constexpr and suitable for dimensional
/// analysis.
///
/// \tparam FactorList Must be a `mult<factor...>`. That is the only supported
/// specialization of this template class.
///
template <class FactorList> class concrete_factor_generator {};

/// The only supported specialization of concrete_factor_generator.
template <factor... Factors> class concrete_factor_generator<mult<Factors...>> {
public:
  /// Unique value that identifies a factor's base type.
  ///
  /// \tparam FactorBase The *base* type of a factor, not the factor type
  /// itself. If the type is not a base of any factor in `Factors...`, then the
  /// value is `UNKNOWN_BASE_ID`.
  ///
  template <class FactorBase>
  constexpr static concrete_factor_base_id base_id = get_base_id<FactorBase>();

  /// The maximum id assigned by this generator.
  constexpr static concrete_factor_base_id max_id = sizeof...(Factors);

  /// Produces an array of concrete factors for each `Factor` in the `mult`
  /// expression.
  constexpr static std::array<concrete_factor, sizeof...(Factors)>
  make_concrete_factors() {
    return {make_concrete_factor<Factors>()...};
  }

private:
  /// Compute the base id for a base type by locating the first index of
  /// `Factors...` whose base is the requested type.
  ///
  /// \tparam FactorBase The *base* type being requested.
  /// \return A unique id for the base type, or `UNKNOWN_BASE_ID` if there is no
  /// factor with the provided base type.
  ///
  template <class FactorBase>
  constexpr static concrete_factor_base_id get_base_id() {
    concrete_factor_base_id id = 0;
    bool found =
        ((++id,
          std::is_same_v<FactorBase, typename factor_traits<Factors>::base>) ||
         ...);
    return found ? (id - 1) : UNKNOWN_BASE_ID;
  }

  /// Construct a concrete factor from a factor type. The factor must appear in
  /// the original `mult` expression passed to this class specialization.
  ///
  template <factor Factor>
  constexpr static concrete_factor make_concrete_factor() {
    concrete_factor f;
    f.base_id = base_id<typename factor_traits<Factor>::base>;
    f.exponent = factor_traits<Factor>::exponent;
    f.is_dimensional = factor_traits<Factor>::is_dimensional;
    f.is_rational_value = factor_traits<Factor>::is_rational_value;
    f.rational_value = factor_traits<Factor>::rational_value;
    f.irrational_value = factor_traits<Factor>::irrational_value;
    return f;
  }
};

} // namespace mu::detail

#endif