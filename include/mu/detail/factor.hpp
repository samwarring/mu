#ifndef INCLUDED_MU_DETAIL_FACTOR_HPP
#define INCLUDED_MU_DETAIL_FACTOR_HPP
#include <mu/detail/ratio.hpp>

namespace mu::detail {

template <class T> struct factor_traits {};

template <class T>
concept factor = requires {
  typename factor_traits<T>::base;
  { factor_traits<T>::exponent } -> std::convertible_to<ratio>;
  { factor_traits<T>::is_dimensional } -> std::convertible_to<bool>;
  { factor_traits<T>::is_rational_value } -> std::convertible_to<bool>;
  { factor_traits<T>::rational_value } -> std::convertible_to<ratio>;
  { factor_traits<T>::irrational_value } -> std::convertible_to<long double>;
};

} // namespace mu::detail

#endif