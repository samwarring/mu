#ifndef INCLUDED_MU_QUANTITY_HPP
#define INCLUDED_MU_QUANTITY_HPP
#include <iostream>
#include <mu/rep.hpp>
#include <mu/units.hpp>
#include <mu/units_conversion.hpp>

namespace mu {

template <class FromRep, class FromUnits, class ToRep, class ToUnits>
concept quantity_losslessly_equivalent_to = requires {
  requires units_equivalent_to<FromUnits, ToUnits>;
  requires rep_losslessly_castable_to<FromRep, ToRep>;
};

template <class FromRep, class FromUnits, class ToRep, class ToUnits>
concept quantity_lossily_equivalent_to = requires {
  requires units_equivalent_to<FromUnits, ToUnits>;
  requires rep_lossily_castable_to<FromRep, ToRep>;
};

template <class FromRep, class FromUnits, class ToRep, class ToUnits>
concept quantity_losslessly_scalable_to = requires {
  requires units_convertible_to<FromUnits, ToUnits>;
  requires rep_losslessly_scalable_to<FromRep, ToRep,
                                      units_conversion_t<FromUnits, ToUnits>>;
};

template <class FromRep, class FromUnits, class ToRep, class ToUnits>
concept quantity_lossily_scalable_to = requires {
  requires units_convertible_to<FromUnits, ToUnits>;
  requires rep_lossily_scalable_to<FromRep, ToRep,
                                   units_conversion_t<FromUnits, ToUnits>>;
};

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

template <class FromRep, class FromUnits, class ToRep, class ToUnits>
concept quantity_lossily_convertible_to = requires {
  requires rep<FromRep>;
  requires rep<ToRep>;
  requires units<FromUnits>;
  requires units<ToUnits>;
  requires quantity_lossily_equivalent_to<FromRep, FromUnits, ToRep, ToUnits> or
               quantity_lossily_scalable_to<FromRep, FromUnits, ToRep, ToUnits>;
};

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

template <rep Rep, units Units> class quantity {
public:
  using rep_type = Rep;
  using units_type = Units;

  constexpr ~quantity() = default;

  constexpr quantity() = default;

  constexpr explicit quantity(Rep value) : value_{std::move(value)} {}

  template <rep FromRep, units FromUnits>
  requires quantity_losslessly_convertible_to<FromRep, FromUnits, Rep, Units>
  constexpr quantity(const quantity<FromRep, FromUnits> &from_quantity)
      : value_{detail::convert_losslessly<Rep, Units, FromUnits>(
            from_quantity.value())} {}

  template <rep FromRep, units FromUnits>
  requires quantity_only_lossily_convertible_to<FromRep, FromUnits, Rep, Units>
  constexpr quantity(const quantity<FromRep, FromUnits> &) = delete;

  template <rep FromRep, units FromUnits>
  requires(!units_convertible_to<FromUnits, ToUnits>)
  constexpr quantity(const quantity<FromRep, FromUnits> &) = delete;

  template <rep FromRep, units FromUnits>
  requires quantity_losslessly_convertible_to<FromRep, FromUnits, Rep, Units>
  constexpr quantity(quantity<FromRep, FromUnits> &&from_quantity)
      : value_{detail::convert_losslessly<Rep, Units, FromUnits>(
            std::move(from_quantity).value())} {}

  template <rep FromRep, units FromUnits>
  requires quantity_only_lossily_convertible_to<FromRep, FromUnits, Rep, Units>
  constexpr quantity(quantity<FromRep, FromUnits> &&) = delete;

  template <rep FromRep, units FromUnits>
  requires(!units_convertible_to<FromUnits, ToUnits>)
  constexpr quantity(quantity<FromRep, FromUnits> &&) = delete;

  template <rep FromRep, units FromUnits>
  requires quantity_losslessly_convertible_to<FromRep, FromUnits, Rep, Units>
  constexpr quantity<Rep, Units> &
  operator=(const quantity<FromRep, FromUnits> &from_quantity) {
    value_ = detail::convert_losslessly<Rep, Units, FromUnits>(
        from_quantity.value());
    return *this;
  }

  template <rep FromRep, units FromUnits>
  requires quantity_losslessly_convertible_to<FromRep, FromUnits, Rep, Units>
  constexpr quantity<Rep, Units> &
  operator=(quantity<FromRep, FromUnits> &&from_quantity) {
    value_ = detail::convert_losslessly<Rep, Units, FromUnits>(
        std::move(from_quantity).value());
    return *this;
  }

  template <rep FromRep, units FromUnits>
  requires quantity_only_lossily_convertible_to<FromRep, FromUnits, Rep, Units>
  constexpr quantity<Rep, Units> &
  operator=(quantity<FromRep, FromUnits> &&) = delete;

  template <rep FromRep, units FromUnits>
  requires(!units_convertible_to<FromUnits, ToUnits>)
  constexpr quantity<Rep, Units> &
  operator=(quantity<FromRep, FromUnits> &&) = delete;

  constexpr Rep value() const & { return value_; }

  constexpr Rep &&value() && { return std::move(value_); }

private:
  Rep value_;
};

template <rep ToRep, units ToUnits, rep FromRep, units FromUnits>
requires quantity_lossily_convertible_to<FromRep, FromUnits, ToRep, ToUnits>
constexpr quantity<ToRep, ToUnits>
quantity_cast(const quantity<FromRep, FromUnits> &from_quantity) {
  return quantity<ToRep, ToUnits>{
      detail::convert_lossily<ToRep, ToUnits, FromUnits>(
          from_quantity.value())};
}

template <rep ToRep, units ToUnits, rep FromRep, units FromUnits>
requires(!units_convertible_to<FromUnits, ToUnits>)
constexpr quantity<ToRep, ToUnits>
quantity_cast(const quantity<FromRep, FromUnits> &) = delete;

template <rep ToRep, units ToUnits, rep FromRep, units FromUnits>
requires quantity_lossily_convertible_to<FromRep, FromUnits, ToRep, ToUnits>
constexpr quantity<ToRep, ToUnits>
quantity_cast(quantity<FromRep, FromUnits> &&from_quantity) {
  return quantity<ToRep, ToUnits>{
      detail::convert_lossily<ToRep, ToUnits, FromUnits>(
          std::move(from_quantity).value())};
}

template <rep ToRep, units ToUnits, rep FromRep, units FromUnits>
requires(!units_convertible_to<FromUnits, ToUnits>)
constexpr quantity<ToRep, ToUnits>
quantity_cast(quantity<FromRep, FromUnits> &&from_quantity) = delete;

template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
requires units_equivalent_to<LhsUnits, RhsUnits>
constexpr bool operator==(const quantity<LhsRep, LhsUnits> &lhs,
                          const quantity<RhsRep, RhsUnits> &rhs) {
  return lhs.value() == rhs.value();
}

template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
requires units_equivalent_to<LhsUnits, RhsUnits>
constexpr bool operator!=(const quantity<LhsRep, LhsUnits> &lhs,
                          const quantity<RhsRep, RhsUnits> &rhs) {
  return lhs.value() != rhs.value();
}

template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
requires units_equivalent_to<LhsUnits, RhsUnits>
constexpr bool operator<(const quantity<LhsRep, LhsUnits> &lhs,
                         const quantity<RhsRep, RhsUnits> &rhs) {
  return lhs.value() < rhs.value();
}

template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
requires units_equivalent_to<LhsUnits, RhsUnits>
constexpr bool operator<=(const quantity<LhsRep, LhsUnits> &lhs,
                          const quantity<RhsRep, RhsUnits> &rhs) {
  return lhs.value() <= rhs.value();
}

template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
requires units_equivalent_to<LhsUnits, RhsUnits>
constexpr bool operator>(const quantity<LhsRep, LhsUnits> &lhs,
                         const quantity<RhsRep, RhsUnits> &rhs) {
  return lhs.value() > rhs.value();
}

template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
requires units_equivalent_to<LhsUnits, RhsUnits>
constexpr bool operator>=(const quantity<LhsRep, LhsUnits> &lhs,
                          const quantity<RhsRep, RhsUnits> &rhs) {
  return lhs.value() >= rhs.value();
}

template <rep Rep, units Units>
constexpr auto operator-(const quantity<Rep, Units> &q) {
  return quantity<Rep, Units>(-q.value());
}

template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
requires units_equivalent_to<LhsUnits, RhsUnits>
constexpr auto operator+(const quantity<LhsRep, LhsUnits> &lhs,
                         const quantity<RhsRep, RhsUnits> &rhs) {
  auto sum_value = lhs.value() + rhs.value();
  using sum_rep = std::remove_cvref_t<decltype(sum_value)>;
  return quantity<sum_rep, LhsUnits>{std::move(sum_value)};
}

template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
requires units_equivalent_to<LhsUnits, RhsUnits>
constexpr auto operator-(const quantity<LhsRep, LhsUnits> &lhs,
                         const quantity<RhsRep, RhsUnits> &rhs) {
  auto difference_value = lhs.value() - rhs.value();
  using difference_rep = std::remove_cvref_t<decltype(difference_value)>;
  return quantity<difference_rep, LhsUnits>{std::move(difference_value)};
}

template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
constexpr auto operator*(const quantity<LhsRep, LhsUnits> &lhs,
                         const quantity<RhsRep, RhsUnits> &rhs) {
  auto product_value = lhs.value() * rhs.value();
  using product_rep = std::remove_cvref_t<decltype(product_value)>;
  using product_units = mu::mult<LhsUnits, RhsUnits>;
  return quantity<product_rep, product_units>{std::move(product_value)};
}

template <rep LhsRep, units LhsUnits, rep RhsRep, units RhsUnits>
constexpr auto operator/(const quantity<LhsRep, LhsUnits> &lhs,
                         const quantity<RhsRep, RhsUnits> &rhs) {
  auto quotient_value = lhs.value() / rhs.value();
  using quotient_rep = std::remove_cvref_t<decltype(quotient_value)>;
  using quotient_units = mu::mult<LhsUnits, mu::pow<RhsUnits, -1>>;
  return quantity<quotient_rep, quotient_units>{std::move(quotient_value)};
}

} // namespace mu

#endif