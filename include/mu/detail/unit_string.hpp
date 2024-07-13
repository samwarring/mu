#ifndef INCLUDED_MU_DETAIL_UNIT_STRING_HPP
#define INCLUDED_MU_DETAIL_UNIT_STRING_HPP
#include <cstdint>
#include <mu/detail/ratio.hpp>
#include <string>
#include <string_view>

namespace mu {

/// Options specify how units should be displayed when formatted as a string.
struct format_options {

  /// Allowed options for displaying named units.
  enum class label_type {
    /// Display named units using their full names.
    names,
    /// Display named units using their symbols.
    symbols
  };

  /// Determines how to display named units.
  label_type labels = label_type::names;

  /// Separator used to diplay units that are multiplied together.
  const char *mult_sep = " * ";
};

namespace detail {

/// Contains all label characteristics of a named unit or constant.
struct labels {
  const char *name = "";
  const char *symbol = "";

  /// A value of true means the named unit (or more likely named constant)
  /// is "attached" to the immediately following named unit in the unit
  /// expression.
  ///
  bool is_prefix = false;
};

/// Builds up a string representation of a unit expression.
///
/// The `unit_string` type is constructed with formatting options that specify
/// how the units are formatted, and passed down the expression tree to each
/// unit, which can add itself to the string by calling various methods on it.
///
class unit_string {
public:
  /// Construct a `unit_string` with a set of formatting options.
  ///
  /// \see format_options which describe all the ways formatting can be
  /// customized.
  ///
  constexpr unit_string(format_options opts)
      : opts_{std::move(opts)}, subexpression_stack_{1} {}

  /// Push a new subexpression into scope.
  ///
  /// Until the next call to `pop`, methods called on the `unit_string` pertain
  /// only to the current subexpression.
  ///
  constexpr void push() { subexpression_stack_.emplace_back(); }

  /// End the current subexpression.
  ///
  /// \warning Users must ensure every `push` has a matching `pop`. They
  /// *especially* must NOT call `pop` more times than they call `push`.
  ///
  constexpr void pop() {
    subexpression top = std::move(back());
    subexpression_stack_.pop_back();
    if (!back().str.empty() && !back().ends_with_prefix) {
      back().str += opts_.mult_sep;
      back().pow_requires_parens = true;
    }
    back().str += top.str;
    back().ends_with_prefix = top.ends_with_prefix;
    back().pow_requires_parens =
        back().pow_requires_parens || top.pow_requires_parens;
  }

  /// Multiply the current subexpression by a named unit or constant.
  ///
  constexpr void multiply(const labels &named_unit) {
    if (!back().str.empty() && !back().ends_with_prefix) {
      back().str += opts_.mult_sep;
      back().pow_requires_parens = true;
    }
    switch (opts_.labels) {
    case format_options::label_type::names:
      back().str += named_unit.name;
      break;
    case format_options::label_type::symbols:
      back().str += named_unit.symbol;
      break;
    default:
      back().str += named_unit.name;
    }
    back().ends_with_prefix = named_unit.is_prefix;
  }

  /// Multiply the current subexpression by a rational value.
  ///
  constexpr void multiply(ratio constant_value) {
    if (!back().str.empty()) {
      back().str += opts_.mult_sep;
      back().pow_requires_parens = true;
    }
    back().str += to_string(constant_value);
    back().ends_with_prefix = false;
  }

  /// Raise the entire current subexpression to a rational power.
  ///
  constexpr void pow(ratio exponent) {
    std::string suffix = "^" + to_string(exponent);
    if (back().pow_requires_parens) {
      back().str = "(" + back().str + ")" + suffix;
    } else {
      back().str += suffix;
    }
    back().pow_requires_parens = true;
  }

  /// Get the formatted unit expression resulting from all previous method
  /// calls.
  ///
  constexpr std::string str() const {
    std::string ret;
    for (const auto &subexpr : subexpression_stack_) {
      ret += subexpr.str;
    }
    return ret;
  }

private:
  /// Describe cosmetic options how the units are formatted.
  format_options opts_;

  /// The `unit_string` maintains a stack of subexpressions.
  struct subexpression {
    /// Formatted subexpression.
    std::string str;

    /// True if the subexpression ends with a named prefix. An immediately
    /// following named unit must not use a mult separator.
    bool ends_with_prefix = false;

    /// True if the subexpression contains a mult separator. If so, raising the
    /// entire subexpression to a power would require enclosing the
    /// subexpression in parentheses.
    bool pow_requires_parens = false;
  };
  std::vector<subexpression> subexpression_stack_;

private:
  /// Gets the top of the subexpression stack.
  constexpr subexpression &back() { return subexpression_stack_.back(); }

  /// Formats the integer as a string. Note `std::to_string` is not sufficient
  /// because it is not constexpr.
  ///
  constexpr static std::string to_string(std::intmax_t value) {
    std::string ret;
    bool negative = (value < 0);
    if (negative) {
      value *= -1;
    };
    while (value > 0) {
      ret += ('0' + value % 10);
      value /= 10;
    };
    if (negative) {
      ret += '-';
    }
    std::reverse(ret.begin(), ret.end());
    return ret;
  }

  /// Formats a ratio as a string. If the denominator of the ratio is 1, then
  /// this only formats the numerator.
  ///
  constexpr static std::string to_string(ratio value) {
    if (value.den == 1) {
      return to_string(value.num);
    } else {
      return to_string(value.num) + "/" + to_string(value.den);
    }
  }
};

} // namespace detail
} // namespace mu

#endif