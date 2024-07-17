#ifndef INCLUDED_MU_DETAIL_UNIT_STRING_HPP
#define INCLUDED_MU_DETAIL_UNIT_STRING_HPP
#include <cstdint>
#include <mu/detail/ratio.hpp>
#include <mu/format_options.hpp>
#include <string>
#include <vector>

namespace mu::detail {

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
    back().str += to_string(constant_value, false);
    back().ends_with_prefix = false;
  }

  /// Raise the entire current subexpression to a rational power.
  ///
  constexpr void pow(ratio exponent) {
    std::string suffix;
    if (opts_.superscript_exponents) {
      suffix = to_string(exponent, true);
    } else {
      suffix = "^" + to_string(exponent, false);
    }
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
  /// because it is not constexpr. If \p superscript is `true`, format digits
  /// using their superscript versions (assume UTF-8).
  ///
  constexpr static std::string to_string(std::intmax_t value,
                                         bool superscript) {
    std::string ret;
    bool negative = (value < 0);
    if (negative) {
      value *= -1;
    };
    while (value > 0) {
      prepend_digit(ret, value % 10, superscript);
      value /= 10;
    };
    if (negative) {
      ret.insert(0, superscript ? "⁻" : "-");
    }
    return ret;
  }

  /// Formats a ratio as a string. If the denominator of the ratio is 1, then
  /// this only formats the numerator. If \p superscript is `true`, format
  /// digits using their superscript versions (assume UTF-8).
  ///
  constexpr static std::string to_string(ratio value, bool superscript) {
    if (value.den == 1) {
      return to_string(value.num, superscript);
    } else {
      return to_string(value.num, superscript) + (superscript ? "ᐟ" : "/") +
             to_string(value.den, superscript);
    }
  }

  constexpr static void prepend_digit(std::string &out, int digit,
                                      bool superscript) {
    if (superscript) {
      const char *glyph;
      switch (digit) {
        // clang-format off
      case 0: glyph = "⁰"; break;
      case 1: glyph = "¹"; break;
      case 2: glyph = "²"; break;
      case 3: glyph = "³"; break;
      case 4: glyph = "⁴"; break;
      case 5: glyph = "⁵"; break;
      case 6: glyph = "⁶"; break;
      case 7: glyph = "⁷"; break;
      case 8: glyph = "⁸"; break;
      case 9: glyph = "⁹"; break;
      default: glyph = "ᵡ"; break;
        // clang-format on
      }
      out.insert(0, glyph);

    } else {
      out.insert(0, 1, static_cast<char>('0' + digit));
    }
  }
};

} // namespace mu::detail

#endif