#ifndef INCLUDED_MU_UNITS_SI_UNITS_HPP
#define INCLUDED_MU_UNITS_SI_UNITS_HPP
#include <mu/pow.hpp>
#include <mu/units.hpp>
#include <mu/units/si_prefixes.hpp>
#include <ratio>

namespace mu {

/// Base units

struct meter {
  constexpr static const char *name = "meter";
  constexpr static const char *symbol = "m";
};

struct second {
  constexpr static const char *name = "second";
  constexpr static const char *symbol = "s";
};

struct gram {
  constexpr static const char *name = "gram";
  constexpr static const char *symbol = "g";
};

struct mole {
  constexpr static const char *name = "mole";
  constexpr static const char *symbol = "mol";
};

struct ampere {
  constexpr static const char *name = "ampere";
  constexpr static const char *symbol = "A";
};

struct kelvin {
  constexpr static const char *name = "kelvin";
  constexpr static const char *symbol = "K";
};

struct candela {
  constexpr static const char *name = "candela";
  constexpr static const char *symbol = "cd";
};

/// Scaled base units

struct minute {
  constexpr static const char *name = "minute";
  constexpr static const char *symbol = "min";
  using units = mult<std::ratio<60>, second>;
};

struct hour {
  constexpr static const char *name = "hour";
  constexpr static const char *symbol = "hr";
  using units = mult<std::ratio<60>, minute>;
};

using nanometer = mult<nano, meter>;
using micrometer = mult<micro, meter>;
using millimeter = mult<milli, meter>;
using centimeter = mult<centi, meter>;
using kilometer = mult<kilo, meter>;

using nanosecond = mult<nano, second>;
using microsecond = mult<micro, second>;
using millisecond = mult<milli, second>;

using milligram = mult<milli, gram>;
using kilogram = mult<kilo, gram>;

using microampere = mult<micro, ampere>;
using milliampere = mult<milli, ampere>;

/// Derived units

using meter_per_second = mult<meter, per<second>>;

struct newton {
  constexpr static const char *name = "newton";
  constexpr static const char *symbol = "N";
  using units = mult<kilogram, meter, per<second, 2>>;
};

struct coulomb {
  constexpr static const char *name = "coulomb";
  constexpr static const char *symbol = "C";
  using units = mult<ampere, second>;
};

struct hertz {
  constexpr static const char *name = "hertz";
  constexpr static const char *symbol = "Hz";
  using units = per<second>;
};

struct joule {
  constexpr static const char *name = "joule";
  constexpr static const char *symbol = "J";
  using units = mult<newton, meter>;
};

struct watt {
  constexpr static const char *name = "watt";
  constexpr static const char *symbol = "W";
  using units = mult<joule, per<second>>;
};

struct volt {
  constexpr static const char *name = "volt";
  constexpr static const char *symbol = "V";
  using units = mult<joule, per<coulomb>>;
};

} // namespace mu

#endif