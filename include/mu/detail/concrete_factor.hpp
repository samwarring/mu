#ifndef INCLUDED_MU_DETAIL_CONCRETE_FACTOR_HPP
#define INCLUDED_MU_DETAIL_CONCRETE_FACTOR_HPP
#include <array>
#include <limits>
#include <mu/detail/factor.hpp>
#include <mu/detail/ratio.hpp>
#include <mu/units.hpp>

namespace mu::detail {

using concrete_factor_base_id = std::size_t;

constexpr concrete_factor_base_id UNKNOWN_BASE_ID =
    std::numeric_limits<concrete_factor_base_id>::max();

struct concrete_factor {
  concrete_factor_base_id base_id = UNKNOWN_BASE_ID;
  ratio exponent = 0;
  bool is_dimensional = false;
  bool is_rational_value = 1;
  ratio rational_value = 1;
  long double irrational_value = 1.0;
};

template <class FactorList> class concrete_factor_generator {};

template <factor... Factors> class concrete_factor_generator<mult<Factors...>> {
public:
  template <class FactorBase>
  constexpr static concrete_factor_base_id base_id = get_base_id<FactorBase>();

  constexpr static concrete_factor_base_id max_id = sizeof...(Factors);

  constexpr static std::array<concrete_factor, sizeof...(Factors)>
  make_concrete_factors() {
    return {make_concrete_factor<Factors>()...};
  }

private:
  template <class FactorBase>
  constexpr static concrete_factor_base_id get_base_id() {
    concrete_factor_base_id id = 0;
    bool found =
        ((++id,
          std::is_same_v<FactorBase, typename factor_traits<Factors>::base>) ||
         ...);
    return found ? (id - 1) : UNKNOWN_BASE_ID;
  }

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